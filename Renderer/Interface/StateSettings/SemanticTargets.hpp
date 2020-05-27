#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class SemanticTargets
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(SemanticTargets)
			
		
		public: virtual const void *TargetPosition() const = 0;

		public: virtual const void *TargetTexcoord() const = 0;

		public: virtual const void *TargetNormal() const = 0;

		public: virtual const void *TargetColor() const = 0;
		
	};
						
	
}

