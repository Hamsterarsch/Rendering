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

				private: static void CheckCreation(HRESULT result);

				private: void MapBufferCompletely(size_t bufferSizeInBytes);

			public: inline virtual void Reset() override { cpuSideAllocPos = cpuSideStart; }
			
			public: inline virtual size_t GetSizeInBytes() const override { return static_cast<size_t>(cpuSideEnd - cpuSideStart); }

			public: inline virtual DxPtr<ID3D12Resource> GetResource() const override { return uploadBuffer; }
			
			public: virtual D3D12_GPU_VIRTUAL_ADDRESS CopyDataToUploadAddress(const void *data, size_t dataByteCount, size_t alignment) override;

				private: bool ThereIsNoCapacityFor(size_t countBytes) const;
			
				private: void CopyToAllocSidePos(const void *data, size_t dataByteCount, size_t alignment);
						 							 							 			
		};
		
		
	}
	
	
}
