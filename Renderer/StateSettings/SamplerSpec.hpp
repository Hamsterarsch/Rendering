#pragma once
#include "StateSettings/FilterTargets.hpp"
#include "StateSettings/AddressingTargets.hpp"


namespace Renderer
{
	struct SamplerSpec
	{
		t_filter_targets filter;
		t_addressing_target addressU;
		t_addressing_target addressV;
		t_addressing_target addressW;
				
	};
	
}
