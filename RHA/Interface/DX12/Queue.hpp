#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DX12/CmdList.hpp"


struct ID3D12CommandQueue;

namespace RHA
{
	namespace DX12
	{
		class Queue
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Queue)

			public: virtual DxPtr<ID3D12CommandQueue> GetQueue() = 0;
			
			public: virtual void SubmitCommandList(CmdList *list) = 0;
						
		};
		
		
	}
	
	
}
