#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class FilterTargets
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(FilterTargets)

		public: virtual const void *FilterMinMagMipLinear() const = 0;
		
	};

	using t_filter_targets = const void *(FilterTargets:: *)() const;
	
	
}
