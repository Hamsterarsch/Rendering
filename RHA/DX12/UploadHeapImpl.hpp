#pragma once
#include "DX12/UploadHeap.hpp"



namespace RHA
{
	namespace DX12
	{		
		class UploadHeapImpl : public UploadHeap
		{
			private: DxPtr<ID3D12Resource> uploadBuffer;

			private: BYTE *cpuSideStart;

			private: BYTE *cpuSideEnd;

			private: BYTE *cpuSideAllocPos;


			
			public: UploadHeapImpl(DeviceResources *resources, size_t sizeInBytes);


			public: virtual D3D12_GPU_VIRTUAL_ADDRESS CopyDataToUploadAddress(const void *data, size_t dataByteCount, size_t alignment) override;
				
				private: void AdvanceAllocPosToAlignment(size_t alignment);

					private: bool AlignmentIsInvalid(size_t alignment);
							 			
		};
		
		
	}
	
	
}
