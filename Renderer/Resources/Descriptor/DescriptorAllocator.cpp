#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include "DX12/DeviceResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Shared/Exception/Exception.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Utility/Alignment.hpp"

namespace Renderer::DX12
{
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
	
	DescriptorAllocator::DescriptorAllocator(DescriptorAllocator &&Other) noexcept :
		view{ std::move(Other.view) },
		sampler{ std::move(Other.sampler) },
		resources{ std::move(Other.resources) },
		parent{ std::move(Other.parent) }
	{			
		Other.view.chunk.capacity = Other.view.chunk.startIndex = 0;					
		Other.sampler.chunk.capacity = Other.sampler.chunk.startIndex = 0;						
		Other.resources = nullptr;
								
	}

	DescriptorAllocator::~DescriptorAllocator() noexcept
	{
		if(parent == nullptr)
		{
			return;
		}
		
		if(view.chunk.capacity > 0)
		{
			parent->RetireViewDescriptorChunk(view.chunk);
		}

		if(sampler.chunk.capacity > 0)
		{
			parent->RetireSamplerDescriptorChunk(sampler.chunk);
		}
					
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

			void DescriptorAllocator::UpdateAfterTableIndex(const ChunkData &forChunkData, const size_t offsetFromTableStartToDescriptor)
			{			
				view.offsetToAfterTable = max(forChunkData.offsetToAfterTable, forChunkData.offsetToTableStart + offsetFromTableStartToDescriptor +1 );

			}


	
	void DescriptorAllocator::CreateSrvBuffer(ID3D12Resource* resource, size_t tableOffset, size_t firstIndex, size_t numElements, size_t strideInBytes)
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
	


	void DescriptorAllocator::CreateCbv
	(
		ID3D12Resource *resource,
		const size_t tableOffset,
		const size_t bufferSizeInBytes
	)
	{
		CheckIfValidOpenTable();
					
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc{ resource->GetGPUVirtualAddress(), bufferSizeInBytes };
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
		CreateUavBufferInternal(resource, nullptr, tableOffset, firstIndex, numElements, strideInBytes);
		
	}

		void DescriptorAllocator::CreateUavBufferInternal(ID3D12Resource *resource, ID3D12Resource *counter, size_t tableOffset, size_t firstIndex, size_t numElements, size_t strideInBytes)
		{
			CheckIfValidOpenTable();

			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = firstIndex;
			uavDesc.Buffer.NumElements = numElements;
			uavDesc.Buffer.StructureByteStride = strideInBytes;

			if(counter != nullptr)
			{
				/*
				uavDesc.Buffer.FirstElement = uavDesc.Buffer.FirstElement == 0 ? 1 : uavDesc.Buffer.FirstElement;
				uavDesc.Buffer.CounterOffsetInBytes = (uavDesc.Buffer.FirstElement-1) * uavDesc.Buffer.StructureByteStride;
				
				if(uavDesc.Buffer.StructureByteStride < 4)
				{
					throw Exception::CreationFailed{"Could not create UAV with counter for buffer because structure byte stride must at least be 4"};	
				}			*/	

				
			}
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
		CreateUavBufferInternal(resource, counter, tableOffset, firstIndex, numElements, strideInBytes);
		
	}

	
}
