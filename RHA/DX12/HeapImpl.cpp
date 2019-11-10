#include "DX12/HeapImpl.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Shared/Exception/OutOfMemoryException.hpp"
#include "DX12/HeapAllocation.hpp"
#include "Utility/Alignment.hpp"


namespace RHA
{
	namespace DX12
	{
		HeapImpl::HeapImpl(DeviceResources *resources, const size_t sizeInBytes, const size_t alignment, const D3D12_HEAP_FLAGS flags) :
			sizeInBytes{ sizeInBytes },
			offsetToFreeRegion{ 0 },
			alignment{ alignment }
		{
			if(AlignmentIsInvalid())
			{
				throw Exception::CreationFailed{ "Tried to create a dx12 heap with invalid alignment" };
			}
			
			D3D12_HEAP_DESC desc{};
			desc.Alignment = alignment;
			desc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
			desc.SizeInBytes = Utility::IncreaseValueToAlignment(sizeInBytes, alignment);
			desc.Flags = flags;
			
			const auto result
			{
				resources->GetDevice()->CreateHeap(&desc, IID_PPV_ARGS(&heap))
			};
			CheckHeapCreation(result);
						
		}

			bool HeapImpl::AlignmentIsInvalid() const
			{
				return alignment == 0 || (alignment & (alignment-1));
			}

			void HeapImpl::CheckHeapCreation(const HRESULT result) const
			{			
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create dx12 heap" };
				}
							
			}

		

		bool HeapImpl::HasCapacityForAllocation(const size_t allocationSizeInBytes) const
		{
			return sizeInBytes - offsetToFreeRegion >= allocationSizeInBytes;
		
		}
		

		
		HeapAllocation HeapImpl::Allocate(const size_t sizeInBytes)
		{
			CheckAllocationSize(sizeInBytes);
			
			HeapAllocation allocation{};
			allocation.heap = heap.Get();
			allocation.offsetToAllocation = offsetToFreeRegion;

			UpdateAllocationOffsets(sizeInBytes);			
			allocation.allocationSize = offsetToFreeRegion - allocation.offsetToAllocation;
			return allocation;
						
		}

			void HeapImpl::CheckAllocationSize(const size_t allocationSizeInBytes) const
			{
				if( ! HasCapacityForAllocation(allocationSizeInBytes))
				{
					throw Exception::OutOfMemory{ "Dx12 heap is out of memory" };
				}
			
			}
		
			void HeapImpl::UpdateAllocationOffsets(const size_t sizeInBytes)
			{				
				offsetToFreeRegion = min(Utility::IncreaseValueToAlignment(offsetToFreeRegion + sizeInBytes, alignment), sizeInBytes);
			
			}

		

		void HeapImpl::Reset()
		{
			offsetToFreeRegion = 0;
			
		}

		
	}

	
}
