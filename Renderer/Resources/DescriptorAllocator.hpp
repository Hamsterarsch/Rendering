#pragma once
#include <d3d12.h>

namespace Renderer
{
	namespace DX12
	{
		class DescriptorAllocator
		{			
			size_t offsetToFreeArea;

			size_t capacity;



			public: DescriptorAllocator(size_t viewCapacity, size_t viewStartIndex, size_t samplerCapacity, size_t samplerStartIndex);
			
			
			public: void OpenTable();
			
			public: D3D12_GPU_DESCRIPTOR_HANDLE CreateDefaultedSrv(ID3D12Resource *resource, size_t tableOffset);

			public: D3D12_GPU_DESCRIPTOR_HANDLE CreateDefaultedCbv(ID3D12Resource *resource, size_t tableOffset);

			public: D3D12_GPU_DESCRIPTOR_HANDLE CreateDefaultedUav(ID3D12Resource *resource, size_t tableOffset);

			public: void CloseTable();
			
		};

		
	}

	
}
