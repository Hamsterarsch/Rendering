#pragma once
#include "StateSettings/FormatTargets.hpp"


namespace Renderer::DX12
{
	class FormatTargetsImpl final : public FormatTargets
	{
		public: const void *R32G32B32_Float() const override;
		
		public: const void *R32G32_Float() const override;

		public: const void *R8G8B8A8_SNorm() const override;
		
		public: const void *R8G8B8A8_UNorm() const override;
		
		public: const void *R16_Uint() const override;
		
		
		public: static const FormatTargetsImpl &Get();

	};
	
	
}