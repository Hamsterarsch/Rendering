#pragma once
#include "Shared/InterfaceHelpers.hpp"


struct ID3D12Fence;


namespace RHA
{
	namespace DX12
	{
		class Fence
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Fence)
				
			
			public: virtual DxPtr<ID3D12Fence> GetFence() = 0;
			
			public: virtual size_t GetValue() const = 0;			
			
			public: virtual void Signal(size_t value) = 0;

			public: virtual void Signal(size_t value, class Queue *queue) = 0;

			public: virtual void SetEventOnValue(size_t triggerValue, HANDLE eventHandle) = 0;
									   			
		};

		
	}

	
}
