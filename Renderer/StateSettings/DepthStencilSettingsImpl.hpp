#pragma once
#include "StateSettings/DepthStencilSettings.hpp"
#include <d3d12.h>


namespace Renderer::DX12
{
	class DepthStencilSettingsImpl final : public DepthStencilSettings
	{
		private: struct State
		{
			bool enableDepth;			
			D3D12_COMPARISON_FUNC depthFunc;

			State();
			
		} current, saved;


		
		public: void ResetToDefault() override;

		public: void SaveSettings() override;

		public: void RestoreSettings() override;

		
		public: void SetEnableDepth(bool value) override;

		public: D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() const;

			private: static constexpr D3D12_DEPTH_STENCILOP_DESC MakeDefaultStencilDesc();
		
	};


}
