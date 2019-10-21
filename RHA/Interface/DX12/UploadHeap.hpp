#pragma once
#include "DX12/DeviceResources.hpp"
#include "Shared/InterfaceHelpers.hpp"


namespace RHA
{
	namespace DX12
	{
		class UploadHeap
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(UploadHeap)


			public: virtual D3D12_GPU_VIRTUAL_ADDRESS CopyDataToUploadAddress(const void *data, size_t dataByteCount, size_t alignment) = 0;
			
		};

		
	}

	
}
