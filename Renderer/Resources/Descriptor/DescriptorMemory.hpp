#pragma once
#include "DX12/DescriptorHeap.hpp"
#include "Shared/PtrTypes.hpp"
#include "DescriptorChunkRegistry.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include <mutex>


namespace RHA
{
	namespace DX12
	{
		class CmdList;
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

			private: DeviceResources *resources;

			private: std::mutex layoutMutex;
					 
			
			
			public: DescriptorMemory(DeviceResources *resources, size_t viewCapacity, size_t samplerCapacity);

			
			public: class DescriptorAllocator GetDescriptorAllocator(size_t viewCapacity, size_t samplerCapacity);

			public: void RetireViewDescriptorChunk(const DescriptorChunk &chunk);

			public: void RetireSamplerDescriptorChunk(const DescriptorChunk &chunk);

			
			public: D3D12_CPU_DESCRIPTOR_HANDLE GetViewHandleCpu(size_t index) const;
			
			public: D3D12_GPU_DESCRIPTOR_HANDLE GetViewHandleGpu(size_t index) const;
			
			public: D3D12_CPU_DESCRIPTOR_HANDLE GetSamplerHandleCpu(size_t index) const;

			public: D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerHandleGpu(size_t index) const;


			public: void RecordListBinding(CmdList *list);
								
		};
			   
		
	}
	
	
}
