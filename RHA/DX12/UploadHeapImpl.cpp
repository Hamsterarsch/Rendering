#include <d3d12.h>
#include "Shared/Exception/CreationFailedException.hpp"
#include "DX12/UploadHeapImpl.hpp"


namespace RHA
{
	namespace DX12
	{
		UploadHeapImpl::UploadHeapImpl(DeviceResources *resources, size_t sizeInBytes)
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

			if (FAILED(result))
			{
				throw Exception::CreationFailed{ "Could not create dx12 upload heap" };
			}

			D3D12_RANGE mappedRange{ 0, 0 };
			void *dataStart;

			uploadBuffer->Map(0, &mappedRange, &dataStart);

			cpuSideStart = cpuSideAllocPos = reinterpret_cast<BYTE *>(dataStart);
			cpuSideEnd = cpuSideStart + sizeInBytes;

		}


		
		D3D12_GPU_VIRTUAL_ADDRESS UploadHeapImpl::CopyDataToUploadAddress(const void *data, size_t dataByteCount, size_t alignment)
		{
			AdvanceAllocPosToAlignment(alignment);

			if (cpuSideAllocPos + dataByteCount > cpuSideEnd)
			{
				throw Exception::Exception{ "Upload heap out of memory" };
			}

			memcpy(cpuSideAllocPos, data, dataByteCount);
			const intptr_t offsetToAllocation = cpuSideAllocPos - cpuSideStart;
			cpuSideAllocPos += dataByteCount;

			return uploadBuffer->GetGPUVirtualAddress() + offsetToAllocation;

		}

			void UploadHeapImpl::AdvanceAllocPosToAlignment(const size_t alignment)
			{
				if (AlignmentIsInvalid(alignment))
				{
					throw Exception::Exception{ "Upload heap alignment is invalid" };
				}

				cpuSideAllocPos = reinterpret_cast<BYTE *>((reinterpret_cast<size_t>(cpuSideAllocPos) + alignment - 1) & ~(alignment - 1));

			}

				bool UploadHeapImpl::AlignmentIsInvalid(const size_t alignment)
				{
					return alignment == 0 || (alignment & (alignment - 1));

				}
		
	}
	
	
}