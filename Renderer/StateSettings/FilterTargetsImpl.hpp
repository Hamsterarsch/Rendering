#pragma once
#include "StateSettings/FilterTargets.hpp"


namespace Renderer::DX12
{
	class FilterTargetsImpl : public FilterTargets
	{
		public: const void *FilterMinMagMipLinear() const override;


		public: static const FilterTargetsImpl &Get();
		
	};

	
}
