#include "DX12/Facade.hpp"
#include "Resources/ResourceFactory.hpp"
#include "Resources/FrameSuballocator.hpp"
#include "Shared/Exception/CreationFailedException.hpp"


using namespace RHA::DX12;

namespace Renderer
{
	FrameSuballocator::FrameSuballocator(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue, ResourceFactory *parent, const unsigned allocatorID) :
		parent{ parent },
		allocatorID{ allocatorID },
		queue{ queue },
		resources{ resources },
		uploadAddress{ 0 }
	{
		uploadBuffer = Facade::MakeUploadHeap(resources, 1'000'000);
		fence = Facade::MakeFence(resources);
		allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
		
		list = allocator->AllocateList();
		list->AsGraphicsList()->Close();

		event = CreateEvent(nullptr, false, true, nullptr);
		
	}

	DxPtr<ID3D12Resource> FrameSuballocator::MakeBuffer(const void *data, size_t sizeInBytes)
	{
		WaitForSingleObject(event, INFINITE);
				
		CopyDataToUploadBuffer(data, sizeInBytes);
		
		auto gpuAllocation{ parent->MakeAllocationForBuffer(sizeInBytes, allocatorID) };
			   				
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
				D3D12_RESOURCE_STATE_COMMON,
				BUFFER_CLEAR_VALUE,
				IID_PPV_ARGS(&gpuResource)
			)
		};
		CheckGpuResourceCreation(result);
		
		auto glist{ GetFreshCmdList() };		
		glist->CopyBufferRegion(gpuResource.Get(), 0, uploadBuffer->GetResource().Get(), 0, sizeInBytes);
		glist->Close();

		SubmitListAndFenceSynchronization(list.get());		
		return gpuResource;
		
	}

		void FrameSuballocator::CopyDataToUploadBuffer(const void *data, const size_t sizeInBytes)
		{							
			if(UploadBufferCanNotFitAllocation(sizeInBytes))
			{
				uploadBuffer = Facade::MakeUploadHeap(resources, sizeInBytes);
			}

			uploadBuffer->Reset();			
			uploadAddress = uploadBuffer->CopyDataToUploadAddress(data, sizeInBytes, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
		
		}

			bool FrameSuballocator::UploadBufferCanNotFitAllocation(const size_t allocationSizeInBytes) const
			{
				return uploadBuffer->GetSizeInBytes() < allocationSizeInBytes;
				
			}
	
		D3D12_RESOURCE_DESC FrameSuballocator::MakeBufferDesc(const size_t sizeInBytes)
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

		void FrameSuballocator::CheckGpuResourceCreation(const HRESULT result)
		{
			if(FAILED(result))
			{
				throw Exception::CreationFailed{ "FrameSuballocator:: could not create a dx12 placed buffer on gpu heap to upload to" };
			}
		
		}

		DxPtr<ID3D12GraphicsCommandList> FrameSuballocator::GetFreshCmdList()
		{
			auto glist{ list->AsGraphicsList() };
			glist->Reset(allocator->GetAllocator().Get(), nullptr);

			return glist;
		
		}

	void FrameSuballocator::SubmitListAndFenceSynchronization(CmdList *list)
	{
		queue->SubmitCommandList(list);
		
		fence->GetFence()->SetEventOnCompletion(1, event);
		fence->Signal(1, queue);
		
	}

	
}
