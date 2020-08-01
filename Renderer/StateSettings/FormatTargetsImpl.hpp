#pragma once
#include "StateSettings/TargetHelpers.hpp"
#include "StateSettings/FormatTargets.hpp"
#include <d3d12.h>


namespace Renderer::DX12
{
	class FormatTargetsImpl final : public FormatTargets
	{
		public: const void *R32G32B32_Float() const override;
		
		public: const void *R32G32_Float() const override;

		public: const void *R8G8B8A8_SNorm() const override;
		
		public: const void *R8G8B8A8_UNorm() const override;

		public: const void *R32_Uint() const override;
		
		public: const void *R16_Uint() const override;
		
		
		public: static const FormatTargetsImpl &Get();

	};

	template<> struct FormatMapping<FormatTargets>{ using mapping = FormatTargetsImpl; using t_dataDeref = DXGI_FORMAT; };
	
	
}
