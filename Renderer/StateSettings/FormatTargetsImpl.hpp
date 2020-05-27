#pragma once
#include "StateSettings/FormatTargets.hpp"


namespace Renderer::DX12
{
	class FormatTargetsImpl final : public FormatTargets
	{
		public: const void *R32G32B32_Float() const override;
		
		public: const void *R32G32_Float() const override;

		
		public: static const FormatTargetsImpl &Get();
		
	};
	
	
}
