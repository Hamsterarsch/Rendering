#pragma once
#include <d3d12.h>
#include "Resources/DescriptorChunk.hpp"

namespace Renderer
{	
	namespace DX12
	{
		class DescriptorAllocator
		{			
			private: struct ChunkData
			{
				DescriptorChunk chunk;
				
				size_t offsetToFreeIndex;
				
			};

			private: ChunkData view, sampler;


			
			public: DescriptorAllocator(const DescriptorChunk &viewChunk, const DescriptorChunk &samplerChunk);
			
			
			public: void OpenTable();
			
			public: D3D12_GPU_DESCRIPTOR_HANDLE CreateDefaultedSrv(ID3D12Resource *resource, size_t tableOffset);

			public: D3D12_GPU_DESCRIPTOR_HANDLE CreateDefaultedCbv(ID3D12Resource *resource, size_t tableOffset);

			public: D3D12_GPU_DESCRIPTOR_HANDLE CreateDefaultedUav(ID3D12Resource *resource, size_t tableOffset);

			public: void CloseTable();
			
		};

		
	}

	
}
