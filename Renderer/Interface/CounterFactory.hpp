#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class CounterFactory
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CounterFactory)


		public: using CounterID = unsigned;

		public: virtual CounterID MakeCounter(size_t initialValue) = 0;

		public: virtual size_t GetCounterValue(CounterID id) const = 0;

		public: virtual size_t WaitForCounterToReach(CounterID counter, size_t thresholdValue) const = 0;
		
	};
	
	
}
