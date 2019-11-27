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
			memory{ RHA::DX12::Facade::MakeDescriptorHeap(resources, type, capacity, true) },
			layout{ capacity }
		{			
		}

		

		DescriptorMemory::DescriptorMemory(DeviceResources *resources, const size_t viewCapacity, const size_t samplerCapacity) :
			view{ resources, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, viewCapacity },
			sampler{ resources, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, samplerCapacity }
		{
		}


		
		DescriptorAllocator DescriptorMemory::GetDescriptorAllocator(const size_t viewCapacity, const size_t samplerCapacity)
		{			
			return { view.layout.Allocate(viewCapacity), sampler.layout.Allocate(samplerCapacity) };
					
		}

		
	}

	
}
