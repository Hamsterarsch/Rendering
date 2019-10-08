#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace RHA
{
	namespace DX12
	{
		class Queue
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Queue)

			public: virtual void SubmitCommandList(class CmdList &list) = 0;
						
		};
		
		
	}
	
	
}
