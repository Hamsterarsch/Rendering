#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DxPtrTypes.hpp"

struct ID3D12GraphicsCommandList;

namespace RHA
{
	namespace DX12
	{
		class CmdList
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CmdList)

			public: virtual DxPtr<ID3D12CommandList> GetList() = 0;
			
			public: virtual DxPtr<ID3D12GraphicsCommandList> AsGraphicsList() = 0;
					
		};

		
	}

	
}
