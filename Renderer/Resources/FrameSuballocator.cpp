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
		resources{ resources }
	{
		uploadHeap = Facade::MakeUploadHeap(resources, 1'000'000);
		fence = Facade::MakeFence(resources);
		allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
		
		list = allocator->AllocateList();
		list->AsGraphicsList()->Close();

		event = CreateEvent(nullptr, false, true, nullptr);
		
	}

	DxPtr<ID3D12Resource> FrameSuballocator::MakeBufferWithData(void *data, size_t sizeInBytes)
	{
		WaitForSingleObject(event, INFINITE);
				
		if(uploadHeap->GetSizeInBytes() < sizeInBytes)
		{
			uploadHeap = Facade::MakeUploadHeap(resources, sizeInBytes);
		}

		uploadHeap->Reset();
		const auto virtualAddress
		{
			uploadHeap->CopyDataToUploadAddress(data, sizeInBytes, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT)
		};

		auto gpuAllocation{ parent->MakeRawAllocationForBuffer(sizeInBytes, allocatorID) };

		D3D12_RESOURCE_DESC desc{};
		desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = sizeInBytes;
		desc.DepthOrArraySize = 1;
		desc.Height = 1;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		
				
		DxPtr<ID3D12Resource> gpuResource;
		const auto result
		{		
			resources->GetDevice()->CreatePlacedResource
			(
				gpuAllocation.heap, 
				gpuAllocation.offsetToAllocation,
				&desc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&gpuResource)
			)
		};

		if(FAILED(result))
		{
			throw Exception::CreationFailed{ "FrameSuballocator:: could not create a dx12 placed buffer to upload to" };
		}
		
		auto glist{ list->AsGraphicsList() };
		glist->Reset(allocator->GetAllocator().Get(), nullptr);
		
		glist->CopyBufferRegion(gpuResource.Get(), 0, uploadHeap->GetResource().Get(), 0, sizeInBytes);
		glist->Close();

		queue->SubmitCommandList(list.get());

		fence->GetFence()->SetEventOnCompletion(1, event);
		fence->Signal(1, queue);
		
		return gpuResource;
		
	}

	DxPtr<ID3D12Resource> FrameSuballocator::MakeVertexBufferForMesh(ResourceHandle handle)
	{
		auto allocation{ parent->FindExistingAllocation(handle) };
		
		
		
	}

	
}
