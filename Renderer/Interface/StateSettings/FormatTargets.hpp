#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class FormatTargets
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(FormatTargets)


		public: virtual const void *R32G32B32_Float() const = 0;

		public: virtual const void *R32G32_Float() const = 0;

		public: virtual const void *R8G8B8A8_Norm() const = 0;
						
	};

	using t_format_target = const void *(FormatTargets:: *)() const;
	
}
