#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include "DX12/DeviceResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Shared/Exception/Exception.hpp"
#include "Utility/Alignment.hpp"
#include "StateSettings/FormatTargetsImpl.hpp"


namespace Renderer::DX12
{
	DescriptorAllocator::ChunkData::ChunkData() :
		offsetToTableStart{ -1 },
		offsetToAfterTable{ 0 }
	{
	}

	DescriptorAllocator::ChunkData::ChunkData(const DescriptorChunk& chunk, int offsetToTableStart, size_t offsetToAfterTable) :
		chunk{ chunk },
		offsetToTableStart{ -1 },
		offsetToAfterTable{ 0 }
	{
	}

	DescriptorAllocator::DescriptorAllocator() :
		resources{ nullptr },
		parent{ nullptr }
	{
	}

	DescriptorAllocator::DescriptorAllocator
	(
		DeviceResources *resources,
		DescriptorMemory *parent,
		const DescriptorChunk &viewChunk,
		const DescriptorChunk &samplerChunk
	) :
		view{ viewChunk, -1, 0 },
		sampler{ samplerChunk, -1, 0 },
		resources{ resources },
		parent{ parent }
	{
	}



	DescriptorAllocator::~DescriptorAllocator() noexcept
	{
		if(!IsInvalid())
		{
			Free();					
			
		}
							
	}

		bool DescriptorAllocator::IsInvalid() const
		{
			return parent == nullptr;
		
		}

		void DescriptorAllocator::Free()
		{			
			if(view.chunk.capacity > 0)
			{
				parent->RetireViewDescriptorChunk(view.chunk);
			}

			if(sampler.chunk.capacity > 0)
			{
				parent->RetireSamplerDescriptorChunk(sampler.chunk);
			}
					
			Invalidate();
		
		}

			void DescriptorAllocator::Invalidate()
			{
				parent = nullptr;
				resources = nullptr;
				sampler = view = ChunkData{ {}, -1, 0 };
			
			}


	
	DescriptorAllocator::DescriptorAllocator(DescriptorAllocator &&other) noexcept : DescriptorAllocator{}
	{			
		*this = std::move(other);
								
	}


	
	DescriptorAllocator &DescriptorAllocator::operator=(DescriptorAllocator &&rhs) noexcept
	{
		if(this == &rhs)
		{
			return *this;
			
		}

		if(!IsInvalid())
		{
			Free();			
		}
		
		view = std::move(rhs.view);
		sampler = std::move(rhs.sampler);
		resources = std::move(rhs.resources);
		parent = std::move(rhs.parent);

		rhs.Invalidate();
		
		return *this;
		
	}


	
	void DescriptorAllocator::OpenNewTable()
	{			
		view.offsetToTableStart = view.offsetToAfterTable;
		sampler.offsetToTableStart = sampler.offsetToAfterTable;
					
	}


	
	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetCurrentTableStartForView() const
	{
		return parent->GetViewHandleGpu(view.chunk.startIndex + view.offsetToTableStart);
		
	}

	
	
	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetCurrentTableStartForSampler() const
	{
		return parent->GetSamplerHandleGpu(sampler.chunk.startIndex + sampler.offsetToTableStart);
		
	}
	

			
	void DescriptorAllocator::CreateDefaultedSrv
	(
		ID3D12Resource *resource,
		const size_t tableOffset
	)
	{
		CreateSrvInternal(resource, tableOffset, nullptr);
							
	}

		void DescriptorAllocator::CreateSrvInternal(ID3D12Resource *resource, const size_t tableOffset, const D3D12_SHADER_RESOURCE_VIEW_DESC *desc)
		{
			CheckIfValidOpenTable();
				
			resources->GetDevice()->CreateShaderResourceView
			(
				resource, 
				desc, 
				GetViewHandleCpu( GetTargetDescriptorIndex(view, tableOffset) )
			);
			
			UpdateAfterTableIndex(view, tableOffset);
		
		}

			void DescriptorAllocator::CheckIfValidOpenTable() const
			{
				if(view.offsetToTableStart < 0 || sampler.offsetToTableStart < 0)
				{
					throw Exception::Exception{ "You need to open a table before creating descriptors with a DX12 descriptor allocator" };
				}
			
			}
	
			size_t DescriptorAllocator::GetTargetDescriptorIndex
			(
				const ChunkData &forChunkData,				
				const size_t offsetFromTableStartToDescriptor
			)
			{
				return forChunkData.chunk.startIndex + forChunkData.offsetToTableStart + offsetFromTableStartToDescriptor;
			
			}

			D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetViewHandleCpu(const size_t index) const
			{
				return parent->GetViewHandleCpu(index);
			
			}

			void DescriptorAllocator::UpdateAfterTableIndex(ChunkData &forChunkData, const size_t offsetFromTableStartToDescriptor) const
			{
				Exception::ThrowIfDebug(forChunkData.chunk.capacity <= offsetFromTableStartToDescriptor, {"DescriptorAllocator: Descriptor index is out of bounds"});
		
				forChunkData.offsetToAfterTable = max(forChunkData.offsetToAfterTable, forChunkData.offsetToTableStart + offsetFromTableStartToDescriptor +1 );

			}

	void DescriptorAllocator::CreateSrvTex2D
	(
		ID3D12Resource *resource,
		const size_t tableOffset,
		const Format format,
		const uint16_t numMips,
		const uint16_t mostDetailedMip
	)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Format = GetTargetValue(format);        
        desc.Texture2D.MipLevels = numMips;
        desc.Texture2D.MostDetailedMip = mostDetailedMip;
        desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		CreateSrvInternal(resource, tableOffset, &desc);
		
	}



	void DescriptorAllocator::CreateSrvBuffer
	(
		ID3D12Resource *resource,
		const size_t tableOffset,
		const size_t firstIndex,
		const size_t numElements,
		const size_t strideInBytes
	)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;

		desc.Buffer.FirstElement = firstIndex;
		desc.Buffer.NumElements = numElements;
		desc.Buffer.StructureByteStride = strideInBytes;
		
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		CreateSrvInternal(resource, tableOffset, &desc);
		
	}


	
	void DescriptorAllocator::CreateSrvBufferFormatted
	(
		ID3D12Resource *resource,
		const size_t tableOffset,
		const size_t firstIndex,
		const size_t numElements,
		const DXGI_FORMAT format)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};				
		desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;

		desc.Buffer.FirstElement = firstIndex;
		desc.Buffer.NumElements = numElements;
				
		desc.Format = format;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		
		CreateSrvInternal(resource, tableOffset, &desc);
		
	}

	

	void DescriptorAllocator::CreateCbv
	(
		ID3D12Resource *resource,
		const size_t tableOffset,
		const size_t bufferSizeInBytes
	)
	{
		CheckIfValidOpenTable();
					
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc{ resource->GetGPUVirtualAddress(), RHA::Utility::IncreaseValueToAlignment(bufferSizeInBytes, 256) };//todo: make such alignments device based and consostently provided
		resources->GetDevice()->CreateConstantBufferView
		(
			&desc,
			GetViewHandleCpu( GetTargetDescriptorIndex(view, tableOffset) ) 
		);
		
		UpdateAfterTableIndex(view, tableOffset);
		
	}

	
	
	void DescriptorAllocator::CreateSampler(const D3D12_SAMPLER_DESC& desc, size_t tableOffset)
	{
		CheckIfValidOpenTable();
					
		resources->GetDevice()->CreateSampler
		(
			&desc,
			parent->GetSamplerHandleCpu( GetTargetDescriptorIndex(sampler, tableOffset) )
		);

		UpdateAfterTableIndex(sampler, tableOffset);
		
	}



	
	void DescriptorAllocator::CreateUavBuffer
	(
		ID3D12Resource *resource,
		const size_t tableOffset,
		const size_t firstIndex,
		const size_t numElements,
		const size_t strideInBytes
	)
	{
		CreateUavBufferInternal(resource, nullptr, tableOffset, firstIndex, numElements, strideInBytes, DXGI_FORMAT_UNKNOWN);
		
	}

		void DescriptorAllocator::CreateUavBufferInternal(ID3D12Resource *resource, ID3D12Resource *counter, size_t tableOffset, size_t firstIndex, size_t numElements, size_t strideInBytes, const DXGI_FORMAT format)
		{
			CheckIfValidOpenTable();

			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = firstIndex;
			uavDesc.Buffer.NumElements = numElements;
			uavDesc.Buffer.StructureByteStride = strideInBytes;
			uavDesc.Format = format;

			resources->GetDevice()->CreateUnorderedAccessView
			(
				resource,
				counter,
				&uavDesc,
				parent->GetViewHandleCpu( GetTargetDescriptorIndex(view, tableOffset) )
			);

			UpdateAfterTableIndex(view, tableOffset);
		
		}


	
	void DescriptorAllocator::CreateUavBufferWithCounter
	(
		ID3D12Resource *resource,
		ID3D12Resource *counter,
		const size_t tableOffset,
		const size_t firstIndex,
		const size_t numElements,
		const size_t strideInBytes
	)
	{
		CreateUavBufferInternal(resource, counter, tableOffset, firstIndex, numElements, strideInBytes, DXGI_FORMAT_UNKNOWN);
		
	}



	void DescriptorAllocator::CreateUavBufferFormatted(ID3D12Resource *resource, const size_t tableOffset, const size_t firstIndex, const size_t numElements, const DXGI_FORMAT format)
	{
		CreateUavBufferInternal(resource, nullptr, tableOffset, firstIndex, numElements, 0, format);

	}

	
}
