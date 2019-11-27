#pragma once
#include "DX12/DescriptorHeap.hpp"
#include "Shared/PtrTypes.hpp"
#include "DescriptorChunkRegistry.hpp"

namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
	}
}

struct ID3D12Resource;

namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;
				
		
		class DescriptorMemory
		{
			private: struct MemoryData
			{
				UniquePtr<DescriptorHeap> memory;
				
				DescriptorChunkRegistry layout;


				MemoryData(DeviceResources *resources, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t capacity);
				
			};

			private: MemoryData view;			
			
			private: MemoryData sampler;
					 
			
			
			public: DescriptorMemory(DeviceResources *resources, size_t viewCapacity, size_t samplerCapacity);

			
			public: class DescriptorAllocator GetDescriptorAllocator(size_t viewCapacity, size_t samplerCapacity);
					
			

						
			
		};



		
	}
	
	
}
