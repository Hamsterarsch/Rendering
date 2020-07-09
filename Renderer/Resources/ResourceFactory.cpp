#include "DX12/Facade.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Resources/ResourceTypes.hpp"
#include "Resources/ResourceFactory.hpp"
#include "Utility/Alignment.hpp"


namespace Renderer::DX12
{
	using namespace RHA::DX12;
	
	ResourceFactory::ResourceFactory(DeviceResources *resources, Queue *queue, UniquePtr<AllocatableGpuMemory> &&bufferMemory, UniquePtr<AllocatableGpuMemory> &&textureMemory) :
		queue{ queue },
		resources{ resources },
		uploadAddress{ 0 },
		bufferMemory{ std::move(bufferMemory) },
		textureMemory{ std::move(textureMemory) }
	{
		uploadBuffer = Facade::MakeUploadHeap(resources, 1'000'000);
		fence = Facade::MakeFence(resources);
		allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);
		
		list = allocator->AllocateList();
		list->StopRecording();

		event = CreateEvent(nullptr, false, true, nullptr);
		
	}

	ResourceFactory::~ResourceFactory() noexcept = default;
	

	
	ResourceAllocation ResourceFactory::MakeBufferWithData(const void *data, const size_t sizeInBytes, const D3D12_RESOURCE_STATES desiredState, const D3D12_RESOURCE_FLAGS flags)
	{
		WaitForSingleObject(event, INFINITE);
		fence->Signal(0);

		auto creationState{ desiredState };
		if(data != nullptr)
		{							
			CopyDataToUploadBuffer(data, sizeInBytes);
			creationState = D3D12_RESOURCE_STATE_COPY_DEST;
		}

		ResourceAllocation outAlloc{ MakePlacedBufferResource(RHA::Utility::IncreaseValueToAlignment(sizeInBytes, 256), flags, creationState) };
		
		ClearCmdList();						
		list->RecordBarrierAliasing(nullptr, outAlloc.resource.Get());

		if(data != nullptr)
		{
			list->RecordCopyBufferRegion(outAlloc.resource.Get(), 0, uploadBuffer->GetResource().Get(), 0, sizeInBytes);
			list->RecordBarrierTransition(outAlloc.resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, desiredState);				
		}
		list->StopRecording();
		
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

		ResourceAllocation ResourceFactory::MakePlacedBufferResource(const size_t sizeInBytes, const D3D12_RESOURCE_FLAGS resourceFlags, const D3D12_RESOURCE_STATES resourceState)
		{
			ResourceAllocation outAlloc{ this, ResourceTypes::Buffer };
			outAlloc.allocation = bufferMemory->Allocate(sizeInBytes);
						
			const auto desc{ MakeBufferDesc(sizeInBytes, resourceFlags) };
			constexpr decltype(nullptr) BUFFER_CLEAR_VALUE{ nullptr };
			const auto result
			{		
				resources->GetDevice()->CreatePlacedResource
				(
					outAlloc.allocation.heap, 
					outAlloc.allocation.offsetToAllocation,
					&desc,
					resourceState,
					BUFFER_CLEAR_VALUE,
					IID_PPV_ARGS(&outAlloc.resource)
				)
			};
			CheckGpuResourceCreation(result);

			return outAlloc;
		
		}
	
			D3D12_RESOURCE_DESC ResourceFactory::MakeBufferDesc(const size_t sizeInBytes, const D3D12_RESOURCE_FLAGS flags)
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
				desc.Flags = flags;
		
				return desc;
			
			}

		void ResourceFactory::CheckGpuResourceCreation(const HRESULT result)
		{
			if(FAILED(result))
			{
				throw Exception::CreationFailed{ "ResourceFactory:: could not create a dx12 placed buffer on gpu heap to upload to" };
			}
		
		}

		void ResourceFactory::ClearCmdList()
		{
			if(FAILED(allocator->Reset()))
			{
				throw Exception::Exception{ "Could not reset dx12 resource factory command allocator" };
			}

			list = allocator->AllocateList();
						
		}

		void ResourceFactory::SubmitListAndFenceSynchronization(CmdList *list)
		{
			queue->SubmitCommandList(list);
			
			fence->SetEventOnValue(1, event);				
			queue->Signal(1, fence.get());
			
		}


	
	ResourceAllocation ResourceFactory::MakeTextureWithData
	(
		const void *data,
		const size_t width,
		const size_t height,
		const D3D12_RESOURCE_STATES desiredState,
		const D3D12_RESOURCE_FLAGS flags
	)
	{
		WaitForSingleObject(event, INFINITE);
		fence->Signal(0);

		D3D12_SUBRESOURCE_FOOTPRINT subresourceSize
		{
			DXGI_FORMAT_R8G8B8A8_UNORM,
			width,
			height,
			1,
			RHA::Utility::IncreaseValueToAlignment(width * 4, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT)
		};


		auto creationState{ desiredState };
		if(data != nullptr)
		{
			const auto sizeInBytes{ subresourceSize.Width * subresourceSize.Height * subresourceSize.Depth * 4 };
			if(UploadBufferCanNotFitAllocation(sizeInBytes))
			{
				uploadBuffer = Facade::MakeUploadHeap(resources, sizeInBytes);
			}			
			uploadBuffer->Reset();
			
			uploadBuffer->CopyTextureDataToUploadAddress(reinterpret_cast<const char *>(data), subresourceSize);
			creationState = D3D12_RESOURCE_STATE_COPY_DEST;
		}
		
		auto outAlloc{ MakePlacedTextureResource(subresourceSize, flags, creationState) };
		

		D3D12_TEXTURE_COPY_LOCATION dstLoc{};
		dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dstLoc.pResource = outAlloc.resource.Get();
		dstLoc.SubresourceIndex = 0;

		D3D12_TEXTURE_COPY_LOCATION srcLoc{};
		srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		srcLoc.pResource = uploadBuffer->GetResource().Get();
		srcLoc.PlacedFootprint.Footprint = subresourceSize;
		srcLoc.PlacedFootprint.Offset = 0;

		ClearCmdList();						
		list->RecordBarrierAliasing(nullptr, outAlloc.resource.Get());

		if(data != nullptr)
		{
			list->AsGraphicsList()->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);
			list->RecordBarrierTransition(outAlloc.resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, desiredState);				
		}
		list->StopRecording();
		
		SubmitListAndFenceSynchronization(list.get());	
		return outAlloc;
		
	}

		ResourceAllocation ResourceFactory::MakePlacedTextureResource
		(
			const D3D12_SUBRESOURCE_FOOTPRINT &subresourceSize,
			const D3D12_RESOURCE_FLAGS resourceFlags,
			const D3D12_RESOURCE_STATES resourceState
		)
		{
			ResourceAllocation outAlloc{ this, ResourceTypes::Texture };
			outAlloc.allocation = textureMemory->Allocate(subresourceSize.Height * subresourceSize.Width * subresourceSize.Depth * 4);
		
			D3D12_RESOURCE_DESC desc{};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Width = subresourceSize.Width;
			desc.Height = subresourceSize.Height;
			desc.DepthOrArraySize = subresourceSize.Depth;
			desc.Flags = resourceFlags;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.MipLevels = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;		
			
			constexpr decltype(nullptr) NO_CLEAR_VALUE{ nullptr };
			const auto result
			{		
				resources->GetDevice()->CreatePlacedResource
				(
					outAlloc.allocation.heap, 
					outAlloc.allocation.offsetToAllocation,
					&desc,
					resourceState,
					NO_CLEAR_VALUE,
					IID_PPV_ARGS(&outAlloc.resource)
				)
			};
			CheckGpuResourceCreation(result);

			return outAlloc;
		
		}



	DxPtr<ID3D12Resource> ResourceFactory::MakeCommittedBuffer
	(
		const size_t sizeInBytes,
		const D3D12_RESOURCE_STATES desiredState,
		const D3D12_HEAP_TYPE heapType,
		const D3D12_HEAP_FLAGS heapFlags,
		const D3D12_RESOURCE_FLAGS bufferFlags
	)
	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = heapType;
					
		const auto desc{ MakeBufferDesc(sizeInBytes, bufferFlags) };
		constexpr decltype(nullptr) BUFFER_CLEAR_VALUE{ nullptr };

		DxPtr<ID3D12Resource> outResource;
		const auto result
		{		
			resources->GetDevice()->CreateCommittedResource
			(
				&heapProperties,
				heapFlags,
				&desc,
				desiredState,
				BUFFER_CLEAR_VALUE,
				IID_PPV_ARGS(&outResource)
			)
		};
		CheckGpuResourceCreation(result);

		return outResource;
		
	}


	void ResourceFactory::Deallocate(ResourceAllocation &allocation, const ResourceTypes type)
	{
		DeallocateInternal(allocation, type);
		
	}

	
}
