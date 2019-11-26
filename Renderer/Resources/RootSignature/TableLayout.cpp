#include "Resources/RootSignature/TableLayout.hpp"
#include "Shared/Exception/Exception.hpp"


namespace Renderer
{
	namespace DX12
	{
		TableLayout::TableLayout(const D3D12_ROOT_DESCRIPTOR_TABLE1 &tableDesc)
		{
			for(size_t index{ 0 }; index < tableDesc.NumDescriptorRanges; ++index)
			{
				AddAllOffsetsInRange(tableDesc.pDescriptorRanges[index]);				
			}

			srvOffsets.shrink_to_fit();
			uavOffsets.shrink_to_fit();
			cbvOffsets.shrink_to_fit();
			
		}

			void TableLayout::AddAllOffsetsInRange(const D3D12_DESCRIPTOR_RANGE1 &range)
			{
				for(size_t index{ 0 }; index < range.NumDescriptors; ++index)
				{
					AddNewDescriptorOffset(range.RangeType, range.OffsetInDescriptorsFromTableStart + index);					
				}
			
			}

				void TableLayout::AddNewDescriptorOffset(const D3D12_DESCRIPTOR_RANGE_TYPE type, const size_t offset)
				{
					switch(type)
					{
					case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
						srvOffsets.push_back(offset);
						break;
					case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
						uavOffsets.push_back(offset);
						break;
					case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
						cbvOffsets.push_back(offset);
						break;
					case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER: break;
					default:
						throw Exception::Exception{ "Missing case handling in dx12 table layout class" };
					}
			
				}
				

		
		size_t TableLayout::GetSrvOffset(const unsigned short ordinal) const 
		{
			return srvOffsets.at(ordinal-1);

		}

		size_t TableLayout::GetUavOffset(const unsigned short ordinal) const 
		{
			return uavOffsets.at(ordinal-1);

		}

		size_t TableLayout::GetCbvOffset(const unsigned short ordinal) const 
		{
			return cbvOffsets.at(ordinal-1);

		}

		
	}
	
	
}
