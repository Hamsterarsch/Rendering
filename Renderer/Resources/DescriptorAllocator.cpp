#include "Resources/DescriptorAllocator.hpp"
#include "DX12/DeviceResources.hpp"
#include "Resources/DescriptorMemory.hpp"
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
		
		DescriptorAllocator::DescriptorAllocator(DescriptorAllocator&& Other) noexcept :
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
			return parent->GetViewHandleGpu(view.offsetToTableStart);
			
		}

		
		
		D3D12_GPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetCurrentTableStartForSampler() const
		{
			return parent->GetSamplerHandleGpu(sampler.offsetToTableStart);
			
		}
		

				
		void DescriptorAllocator::CreateDefaultedSrv
		(
			ID3D12Resource *resource,
			const size_t tableOffset
		)
		{
			CheckIfValidOpenTable();

			const auto descriptorIndex{ view.offsetToTableStart + tableOffset };
			resources->GetDevice()->CreateShaderResourceView(resource, nullptr, GetViewHandleCpu(descriptorIndex));
			
			UpdateAfterTableIndexForView(descriptorIndex);
						
		}

			void DescriptorAllocator::CheckIfValidOpenTable() const
			{
				if(view.offsetToTableStart < 0 || sampler.offsetToTableStart < 0)
				{
					throw Exception::Exception{ "You need to open a table before creating descriptors with a DX12 descriptor allocator" };
				}
			
			}

			D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetViewHandleCpu(const size_t index) const
			{
				return parent->GetViewHandleCpu(index);
			
			}

			void DescriptorAllocator::UpdateAfterTableIndexForView(const size_t newDescriptorIndex)
			{
				view.offsetToAfterTable = max(view.offsetToAfterTable, newDescriptorIndex +1 );

			}

		
		
		void DescriptorAllocator::CreateCbv
		(
			ID3D12Resource *resource,
			const size_t tableOffset,
			const size_t bufferSizeInBytes
		)
		{
			CheckIfValidOpenTable();
			
			const auto descriptorIndex{ view.offsetToTableStart + tableOffset };
			D3D12_CONSTANT_BUFFER_VIEW_DESC desc{ resource->GetGPUVirtualAddress(), bufferSizeInBytes };
			resources->GetDevice()->CreateConstantBufferView(&desc, GetViewHandleCpu(descriptorIndex));
			
			UpdateAfterTableIndexForView(descriptorIndex);
			
		}

		
		
		void DescriptorAllocator::CreateSampler(const D3D12_SAMPLER_DESC& desc, size_t tableOffset)
		{
			CheckIfValidOpenTable();

			const auto descriptorIndex{ sampler.offsetToTableStart + tableOffset };
			resources->GetDevice()->CreateSampler(&desc, parent->GetSamplerHandleCpu(descriptorIndex));

			sampler.offsetToAfterTable = max(sampler.offsetToAfterTable, descriptorIndex +1);
			
		}

		
	}

	
}
