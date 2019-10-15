#pragma once
#include <d3d12.h>
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/PtrTypes.hpp"
#include "DxPtrTypes.hpp"
#include "CmdList.hpp"


namespace RHA
{
	namespace DX12
	{
		class CmdAllocator
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CmdAllocator)

			
			public: virtual D3D12_COMMAND_LIST_TYPE GetType() const = 0;
			
			public: virtual DxPtr<ID3D12CommandAllocator> GetAllocator() = 0;
			
			public: virtual UniquePtr<CmdList> AllocateList() = 0;
						
		};

		
	}

	
}
