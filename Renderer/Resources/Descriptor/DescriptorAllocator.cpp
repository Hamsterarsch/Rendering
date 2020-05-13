#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include "DX12/DeviceResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Shared/Exception/Exception.hpp"

namespace Renderer
{
	namespace DX12
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
			CheckIfValidOpenTable();
						
			resources->GetDevice()->CreateShaderResourceView
			(
				resource, 
				nullptr, 
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



		
		void DescriptorAllocator::CreateUavBuffer(ID3D12Resource *resource, const size_t tableOffset, const size_t firstIndex, const size_t numElements, const size_t strideInBytes)
		{
			CheckIfValidOpenTable();

			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = firstIndex;
			uavDesc.Buffer.NumElements = numElements;
			uavDesc.Buffer.StructureByteStride = strideInBytes;
						
			resources->GetDevice()->CreateUnorderedAccessView
			(
				resource,
				nullptr,
				&uavDesc,
				parent->GetViewHandleCpu( GetTargetDescriptorIndex(view, tableOffset) )
			);

			UpdateAfterTableIndex(view, tableOffset);
			
		}

		
	}

	
}
