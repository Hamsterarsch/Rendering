#include "Resources/DescriptorMemory.hpp"
#include "Resources/DescriptorAllocator.hpp"
#include "Shared/Exception/Exception.hpp"
#include "DX12/Facade.hpp"

namespace Renderer
{
	namespace DX12
	{
		DescriptorMemory::MemoryData::MemoryData
		(
			DeviceResources *resources, 
			D3D12_DESCRIPTOR_HEAP_TYPE type,
			size_t capacity
		) :
			memory{ Facade::MakeDescriptorHeap(resources, type, capacity, true) },
			layout{ capacity }			
		{			
		}

		

		DescriptorMemory::DescriptorMemory(DeviceResources *resources, const size_t viewCapacity, const size_t samplerCapacity) :
			view{ resources, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, viewCapacity },
			sampler{ resources, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, samplerCapacity },
			resources{ resources }
		{
		}


		
		DescriptorAllocator DescriptorMemory::GetDescriptorAllocator(const size_t viewCapacity, const size_t samplerCapacity)
		{
			std::lock_guard<std::mutex> lock{ layoutMutex };
			return { resources, this, view.layout.Allocate(viewCapacity), sampler.layout.Allocate(samplerCapacity) };
					
		}

		void DescriptorMemory::RetireViewDescriptorChunk(const DescriptorChunk &chunk)
		{
			std::lock_guard<std::mutex> lock{ layoutMutex };
			view.layout.Deallocate(chunk);
			
		}

		void DescriptorMemory::RetireSamplerDescriptorChunk(const DescriptorChunk &chunk)
		{
			std::lock_guard<std::mutex> lock{ layoutMutex };
			sampler.layout.Deallocate(chunk);
			
		}


		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorMemory::GetViewHandleCpu(const size_t index) const
		{
			return view.memory->GetHandleCpu(index);
			
		}

		D3D12_GPU_DESCRIPTOR_HANDLE DescriptorMemory::GetViewHandleGpu(const size_t index) const
		{
			return view.memory->GetHandleGpu(index);
			
		}
		
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorMemory::GetSamplerHandleCpu(const size_t index) const
		{
			return sampler.memory->GetHandleCpu(index);
		}

		D3D12_GPU_DESCRIPTOR_HANDLE DescriptorMemory::GetSamplerHandleGpu(const size_t index) const
		{
			return sampler.memory->GetHandleGpu(index);
			
		}
		
		
	}

	
}
