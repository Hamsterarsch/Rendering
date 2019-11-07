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


			public: inline virtual void Reset() override { cpuSideAllocPos = cpuSideStart; }
			
			public: inline virtual size_t GetSizeInBytes() const override { return static_cast<size_t>(cpuSideEnd - cpuSideStart); }

			public: virtual DxPtr<ID3D12Resource> GetResource() const override { return uploadBuffer; }
			
			public: virtual D3D12_GPU_VIRTUAL_ADDRESS CopyDataToUploadAddress(const void *data, size_t dataByteCount, size_t alignment) override;
				
				private: void AdvanceAllocPosToAlignment(size_t alignment);

					private: bool AlignmentIsInvalid(size_t alignment);
							 							 			
		};
		
		
	}
	
	
}
