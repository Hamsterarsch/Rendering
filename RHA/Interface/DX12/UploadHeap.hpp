#pragma once
#include "DX12/DeviceResources.hpp"
#include "Shared/InterfaceHelpers.hpp"

struct ID3D12Resource;

namespace RHA
{
	namespace DX12
	{
		class UploadHeap
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(UploadHeap)


			public: virtual void Reset() = 0;
			
			public: virtual size_t GetSizeInBytes() const = 0;

			public: virtual DxPtr<ID3D12Resource> GetResource() const = 0;

			public: virtual D3D12_GPU_VIRTUAL_ADDRESS CopyDataToUploadAddress(const void *data, size_t dataByteCount, size_t alignment) = 0;

					
		};

		
	}

	
}
