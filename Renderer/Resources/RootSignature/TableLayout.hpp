#pragma once
#include <vector>
#include <d3d12.h>


namespace Renderer
{
	namespace DX12
	{
		class TableLayout
		{
			private: std::vector<size_t> srvOffsets, uavOffsets, cbvOffsets;


						
			public: TableLayout(const D3D12_ROOT_DESCRIPTOR_TABLE1 *tableDesc);

				private: void AddAllOffsetsInRange(const D3D12_DESCRIPTOR_RANGE1 &range);

					private: void AddNewDescriptorOffset(D3D12_DESCRIPTOR_RANGE_TYPE type, size_t offset);

			
			public: size_t GetSrvOffset(unsigned short ordinal) const;

			public: size_t GetUavOffset(unsigned short ordinal) const;

			public: size_t GetCbvOffset(unsigned short ordinal) const;
					   			
		};

		
	}

	
}
