#include "DX12/Facade.hpp"
#include "Resources/ResourceFactory.hpp"
#include "Shared/Exception/CreationFailedException.hpp"




namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;
		
		ResourceFactory::ResourceFactory(DeviceResources *resources, Queue *queue) :
			queue{ queue },
			resources{ resources },
			uploadAddress{ 0 },
			bufferHeaps{ resources, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 15, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS }
		{
			uploadBuffer = Facade::MakeUploadHeap(resources, 1'000'000);
			fence = Facade::MakeFence(resources);
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
			
			list = allocator->AllocateList();
			list->AsGraphicsList()->Close();

			event = CreateEvent(nullptr, false, true, nullptr);
			
		}

		
		void ResourceFactory::MakeResident() const
		{			
			bufferHeaps.MakeResident();			
			
		}

		
		DxPtr<ID3D12Resource> ResourceFactory::MakeBufferWithData(const void *data, const size_t sizeInBytes, const D3D12_RESOURCE_STATES desiredState)
		{
			WaitForSingleObject(event, INFINITE);
			fence->Signal(0);
			
			CopyDataToUploadBuffer(data, sizeInBytes);
			
			auto gpuAllocation{ bufferHeaps.Allocate(sizeInBytes) };
				   				
			DxPtr<ID3D12Resource> gpuResource;
			const auto desc{ MakeBufferDesc(sizeInBytes) };
			constexpr decltype(nullptr) BUFFER_CLEAR_VALUE{ nullptr };
			const auto result
			{		
				resources->GetDevice()->CreatePlacedResource
				(
					gpuAllocation.heap, 
					gpuAllocation.offsetToAllocation,
					&desc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					BUFFER_CLEAR_VALUE,
					IID_PPV_ARGS(&gpuResource)
				)
			};
			CheckGpuResourceCreation(result);
			
			auto glist{ GetFreshCmdList() };
			
			D3D12_RESOURCE_BARRIER activationBarrier{};
			activationBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
			activationBarrier.Aliasing.pResourceBefore = nullptr;
			activationBarrier.Aliasing.pResourceAfter = gpuResource.Get();
			glist->ResourceBarrier(1, &activationBarrier);
			
			glist->CopyBufferRegion(gpuResource.Get(), 0, uploadBuffer->GetResource().Get(), 0, sizeInBytes);
						
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = gpuResource.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = desiredState;
			glist->ResourceBarrier(1, &barrier);
			
			glist->Close();

			SubmitListAndFenceSynchronization(list.get());		
			return gpuResource;
			
		}

			void ResourceFactory::CopyDataToUploadBuffer(const void *data, const size_t sizeInBytes)
			{							
				if(UploadBufferCanNotFitAllocation(sizeInBytes))
				{
					uploadBuffer = Facade::MakeUploadHeap(resources, sizeInBytes);
				}

				uploadBuffer->Reset();			
				uploadAddress = uploadBuffer->CopyDataToUploadAddress(data, sizeInBytes, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
			
			}

				bool ResourceFactory::UploadBufferCanNotFitAllocation(const size_t allocationSizeInBytes) const
				{
					return uploadBuffer->GetSizeInBytes() < allocationSizeInBytes;
					
				}
		
			D3D12_RESOURCE_DESC ResourceFactory::MakeBufferDesc(const size_t sizeInBytes)
			{
				D3D12_RESOURCE_DESC desc{};
				desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
				desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
				desc.Width = sizeInBytes;
				desc.DepthOrArraySize = 1;
				desc.Height = 1;
				desc.MipLevels = 1;
				desc.SampleDesc.Count = 1;
				desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

				return desc;
			
			}

			void ResourceFactory::CheckGpuResourceCreation(const HRESULT result)
			{
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "FrameSuballocator:: could not create a dx12 placed buffer on gpu heap to upload to" };
				}
			
			}

			DxPtr<ID3D12GraphicsCommandList> ResourceFactory::GetFreshCmdList()
			{
				auto glist{ list->AsGraphicsList() };
				glist->Reset(allocator->GetAllocator().Get(), nullptr);

				return glist;
			
			}

			void ResourceFactory::SubmitListAndFenceSynchronization(CmdList *list)
			{
				queue->SubmitCommandList(list);
				
				fence->SetEventOnValue(1, event);				
				queue->Signal(1, fence.get());
				
			}

			
	}
	
}
