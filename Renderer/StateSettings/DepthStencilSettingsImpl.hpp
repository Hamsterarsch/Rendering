#pragma once
#include "StateSettings/DepthStencilSettings.hpp"
#include <d3d12.h>
#include "StateImpl.hpp"


namespace Renderer::DX12
{
	class DepthStencilSettingsImpl final : public DepthStencilSettings
	{
		private: struct State
		{
			bool enableDepth;			
			D3D12_COMPARISON_FUNC depthFunc;

			State();
			
		};

		private: StateImpl<State> state;
		

		
		public:	void ResetAllToDefault() override { state.ResetAllToDefault(); }

		public: void SaveSettings() override { state.SaveSettings(); }

		public: void RestoreSettingsToSaved() override { state.RestoreSettingsToSaved(); }
		
		public: void RestoreSettingsToDefault() override { state.RestoreSettingsToDefault(); }

		
		public: void SetEnableDepth(bool value) override;

		public: D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() const;

			private: static constexpr D3D12_DEPTH_STENCILOP_DESC MakeDefaultStencilDesc();
		
	};


}
