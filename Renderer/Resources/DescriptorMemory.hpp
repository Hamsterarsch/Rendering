#pragma once
#include "DX12/DescriptorHeap.hpp"
#include "Shared/PtrTypes.hpp"

struct ID3D12Resource;

namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;

		
		
		class DescriptorMemory
		{
			private: UniquePtr<DescriptorHeap> memoryView;
			
			private: UniquePtr<DescriptorHeap> memorySampler;


			
			
			public: DescriptorMemory();

			public: class DescriptorAllocator GetDescriptorAllocator(size_t viewCapacity, size_t samplerCapacity);
					
			

						
			
		};



		
	}
	
	
}
