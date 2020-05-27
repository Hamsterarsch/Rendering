#include <d3d12.h>
#include <cstring>
#include "Shared/Exception/CreationFailedException.hpp"
#include "Shared/Exception/OutOfMemoryException.hpp"
#include "DX12/UploadHeapImpl.hpp"
#include "Utility/Alignment.hpp"


namespace RHA::DX12
{	
	UploadHeapImpl::UploadHeapImpl(DeviceResources *resources, const size_t sizeInBytes)
	{
		D3D12_HEAP_PROPERTIES heapDesc{};
		heapDesc.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC bufferDesc{};
		bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
		bufferDesc.Width = sizeInBytes;
		bufferDesc.Height = 1;
		bufferDesc.DepthOrArraySize = 1;
		bufferDesc.MipLevels = 1;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		const auto result
		{
			resources->GetDevice()->CreateCommittedResource
			(
				&heapDesc,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&uploadBuffer)
			)
		};
		CheckCreation(result);
		
		MapBufferCompletely(sizeInBytes);

	}

		void UploadHeapImpl::CheckCreation(const HRESULT result)
		{
			if (FAILED(result))
			{
				throw Exception::CreationFailed{ "Could not create dx12 upload heap" };
			}
		}

		void UploadHeapImpl::MapBufferCompletely(const size_t bufferSizeInBytes)
		{
			D3D12_RANGE mappedRange{ 0, 0 };
			void *dataStart;

			uploadBuffer->Map(0, &mappedRange, &dataStart);

			cpuSideStart = cpuSideAllocPos = reinterpret_cast<BYTE *>(dataStart);
			cpuSideEnd = cpuSideStart + bufferSizeInBytes;
		
		}


	
	D3D12_GPU_VIRTUAL_ADDRESS UploadHeapImpl::CopyDataToUploadAddress(const void *data, const size_t dataByteCount, const size_t alignment)
	{
		if (ThereIsNoCapacityFor(dataByteCount))
		{
			throw Exception::Exception{ "Upload heap out of memory" };
		}

		if (Utility::AlignmentIsInvalid(alignment))
		{
			throw Exception::OutOfMemory{ "Upload heap alignment is invalid" };
		}

		const intptr_t offsetToAllocation{ cpuSideAllocPos - cpuSideStart };
		const auto gpuAllocationStart{ uploadBuffer->GetGPUVirtualAddress() + offsetToAllocation };
					
		std::memcpy(cpuSideAllocPos, data, dataByteCount);
		AdjustAllocationOffsets(dataByteCount, alignment);
		
		return gpuAllocationStart;
		
	}

		bool UploadHeapImpl::ThereIsNoCapacityFor(const size_t countBytes) const
		{
			return cpuSideAllocPos + countBytes > cpuSideEnd;
		
		}

		void UploadHeapImpl::AdjustAllocationOffsets(size_t dataByteCount, size_t alignment)
		{
			cpuSideAllocPos += dataByteCount;
			cpuSideAllocPos = reinterpret_cast<BYTE *>(Utility::IncreaseValueToAlignment(reinterpret_cast<size_t>(cpuSideAllocPos), alignment));
		
		}



	D3D12_GPU_VIRTUAL_ADDRESS UploadHeapImpl::CopyTextureDataToUploadAddress(const char *data, const D3D12_SUBRESOURCE_FOOTPRINT &resourceInfo)
	{
		if(ThereIsNoCapacityFor(resourceInfo.Width * resourceInfo.Height * resourceInfo.Depth * 32))
		{
			throw Exception::Exception{ "Upload heap out of memory" };
		}
		

		const intptr_t offsetToAllocation{ cpuSideAllocPos - cpuSideStart };

		for(unsigned scanlineIndex{ 0 }; scanlineIndex < resourceInfo.Height; ++scanlineIndex)
		{
			unsigned char *uploadPos{ cpuSideAllocPos + scanlineIndex * resourceInfo.RowPitch };
			const char *copyPos{ &data[scanlineIndex * resourceInfo.Width * 4] };
			std::memcpy(uploadPos, copyPos, resourceInfo.Width * 32);
		}
				
		const auto gpuAllocationStart{ uploadBuffer->GetGPUVirtualAddress() + offsetToAllocation };

		AdjustAllocationOffsets(resourceInfo.Width * resourceInfo.Height * resourceInfo.Depth * 4, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
		return gpuAllocationStart;
		
	}

	
	
	
	
}
