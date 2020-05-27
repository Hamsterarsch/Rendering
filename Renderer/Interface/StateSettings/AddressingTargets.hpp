#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class AddressingTargets
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(AddressingTargets)

		
		public: virtual const void *AddressingModeWrap() const = 0;

		public: virtual const void *AddressingModeClamp() const = 0;

		public: virtual const void *AddressingModeMirror() const = 0;
							   		
	};

	using t_addressing_target = const void *(AddressingTargets:: *)() const;

	
}