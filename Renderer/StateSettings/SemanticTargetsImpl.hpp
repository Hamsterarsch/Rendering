#pragma once
#include "StateSettings/SemanticTargets.hpp"


namespace Renderer::DX12
{
	class SemanticTargetsImpl final : public SemanticTargets
	{
		public: const void *TargetPosition() const override;
		
		public: const void *TargetTexcoord() const override;
				
		public: const void *TargetNormal() const override;
		
		public: const void *TargetColor() const override;

		
		public: static const SemanticTargetsImpl &Get();
		
	};
	
	
}
