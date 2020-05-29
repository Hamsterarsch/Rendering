#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class FormatTargets
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(FormatTargets)


		public: virtual const void *R32G32B32_Float() const = 0;

		public: virtual const void *R32G32_Float() const = 0;

		public: virtual const void *R8G8B8A8_SNorm() const = 0;

		public: virtual const void *R8G8B8A8_UNorm() const = 0;

		public: virtual const void *R16_Uint() const = 0;
						
	};

	using t_format_target = const void *(FormatTargets:: *)() const;
	
}
