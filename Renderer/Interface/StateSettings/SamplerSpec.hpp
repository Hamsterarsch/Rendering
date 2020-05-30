#pragma once
#include "StateSettings/FilterTargets.hpp"
#include "StateSettings/AddressingTargets.hpp"


namespace Renderer
{
	struct SamplerSpec
	{
		Filter filter;
		AddressingMode addressU;
		AddressingMode addressV;
		AddressingMode addressW;
				
	};
	
}
