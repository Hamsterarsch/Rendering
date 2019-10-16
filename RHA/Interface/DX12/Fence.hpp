#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace RHA
{
	namespace DX12
	{
		class Fence
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Fence)
				
			
			public: virtual size_t GetValue() const = 0;
			
			public: virtual void Signal(size_t value) = 0;

			public: virtual void Signal(size_t value, class Queue *queue) = 0;
									   			
		};

		
	}

	
}
