#include "DX12/Facade.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Resources/ResourceTypes.hpp"
#include "Resources/ResourceFactory.hpp"




namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;
		
		ResourceFactory::ResourceFactory(DeviceResources *resources, Queue *queue, UniquePtr<AllocatableGpuMemory> &&memory) :
			queue{ queue },
			resources{ resources },
			uploadAddress{ 0 },
			memory{ std::move(memory) }
		{
			uploadBuffer = Facade::MakeUploadHeap(resources, 1'000'000);
			fence = Facade::MakeFence(resources);
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
			
			list = allocator->AllocateList();
			list->StopRecording();

			event = CreateEvent(nullptr, false, true, nullptr);
			
		}

		ResourceFactory::~ResourceFactory() noexcept = default;
		
	
		
		ResourceAllocation ResourceFactory::MakeBufferWithData(const void *data, const size_t sizeInBytes, const D3D12_RESOURCE_STATES desiredState)
		{
			WaitForSingleObject(event, INFINITE);
			fence->Signal(0);
			
			CopyDataToUploadBuffer(data, sizeInBytes);

			ResourceAllocation outAlloc{ this, ResourceTypes::Buffer };
			outAlloc.allocation = memory->Allocate(sizeInBytes);
						
			const auto desc{ MakeBufferDesc(sizeInBytes) };
			constexpr decltype(nullptr) BUFFER_CLEAR_VALUE{ nullptr };
			const auto result
			{		
				resources->GetDevice()->CreatePlacedResource
				(
					outAlloc.allocation.heap, 
					outAlloc.allocation.offsetToAllocation,
					&desc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					BUFFER_CLEAR_VALUE,
					IID_PPV_ARGS(&outAlloc.resource)
				)
			};
			CheckGpuResourceCreation(result);
			
			auto glist{ GetFreshCmdList() };
			
			D3D12_RESOURCE_BARRIER activationBarrier{};
			activationBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
			activationBarrier.Aliasing.pResourceBefore = nullptr;
			activationBarrier.Aliasing.pResourceAfter = outAlloc.resource.Get();
			glist->ResourceBarrier(1, &activationBarrier);
			
			glist->CopyBufferRegion(outAlloc.resource.Get(), 0, uploadBuffer->GetResource().Get(), 0, sizeInBytes);
						
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = outAlloc.resource.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = desiredState;
			glist->ResourceBarrier(1, &barrier);
			
			glist->Close();

			SubmitListAndFenceSynchronization(list.get());		
			return outAlloc;
			
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
					throw Exception::CreationFailed{ "ResourceFactory:: could not create a dx12 placed buffer on gpu heap to upload to" };
				}
			
			}

			DxPtr<ID3D12GraphicsCommandList> ResourceFactory::GetFreshCmdList()
			{
				if(FAILED(allocator->Reset()))
				{
					throw Exception::Exception{ "Could not reset dx12 resource factory command allocator" };
				}

				list = allocator->AllocateList();
				return list->AsGraphicsList();
			
			}

			void ResourceFactory::SubmitListAndFenceSynchronization(CmdList *list)
			{
				queue->SubmitCommandList(list);
				
				fence->SetEventOnValue(1, event);				
				queue->Signal(1, fence.get());
				
			}

		
	}

	
}
