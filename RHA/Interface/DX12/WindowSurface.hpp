#pragma once
#include "Shared/InterfaceHelpers.hpp"


struct ID3D12GraphicsCommandList;


namespace RHA
{
	namespace DX12
	{
		class WindowSurface
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(WindowSurface)


			public: virtual unsigned GetBufferCount() const = 0;
			
			public: virtual void ScheduleBackbufferClear(Queue *queue) = 0;

			public: virtual void SchedulePresentation(Queue *queue) = 0;
							
		};

		
	}

	
}
