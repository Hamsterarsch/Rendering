#include "Dx12/HeapImpl.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Shared/Exception/OutOfMemoryException.hpp"
#include "DX12/HeapAllocation.hpp"


namespace RHA
{
	namespace DX12
	{
		HeapImpl::HeapImpl(DeviceResources *resources, const size_t sizeInBytes, const size_t alignment) :
			sizeInBytes{ sizeInBytes },
			offsetToFreeRegion{ 0 },
			alignment{ alignment }
		{
			D3D12_HEAP_DESC desc{};
			desc.Alignment = alignment;
			desc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
			desc.SizeInBytes = sizeInBytes;
			
			const auto result
			{
				resources->GetDevice()->CreateHeap(&desc, IID_PPV_ARGS(&heap))
			};
			CheckHeapCreation(result);
						
		}

			void HeapImpl::CheckHeapCreation(const HRESULT result) const
			{
				if(AlignmentIsInvalid())
				{
					throw Exception::CreationFailed{ "Tried to create a dx12 heap with invalid alignment" };
				}
			
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create dx12 heap" };
				}
							
			}

				bool HeapImpl::AlignmentIsInvalid() const
				{
					return alignment == 0 || (alignment & (alignment-1));
				}
				

		
		HeapAllocation HeapImpl::Allocate(const size_t sizeInBytes)
		{
			AdvanceOffsetToFreeRegionToAlignment();
			
			HeapAllocation allocation{};
			allocation.heap = heap.Get();
			allocation.allocationSize = sizeInBytes;
			allocation.offsetToAllocation = offsetToFreeRegion;

			offsetToFreeRegion += sizeInBytes;
			
			CheckHeapState();
			
			return allocation;
						
		}

			void HeapImpl::AdvanceOffsetToFreeRegionToAlignment()
			{
				offsetToFreeRegion = (offsetToFreeRegion + alignment-1) & ~(alignment-1);
			
			}
		
			void HeapImpl::CheckHeapState() const
			{
				if(HeapIsOutOfMemory())
				{
					throw Exception::OutOfMemory{ "Dx12 heap is out of memory" };
				}
			
			}

			bool HeapImpl::HeapIsOutOfMemory() const
			{
				return sizeInBytes - offsetToFreeRegion <= 0;
			
			}
					   
		
	}

	
}
