#pragma once
#include "StateSettings/RasterizerSettings.hpp"
#include <d3d12.h>
#include "StateImpl.hpp"


namespace Renderer::DX12
{
	class RasterizerSettingsImpl final : public RasterizerSettings
	{
		private: struct State
		{
			bool frontIsCounterClockwise;
			D3D12_FILL_MODE fillMode;
			D3D12_CULL_MODE cullMode;

			
			State();
			
		};

		private: StateImpl<State> state;


		
		public:	void ResetAllToDefault() override { state.ResetAllToDefault(); }

		public: void SaveSettings() override { state.SaveSettings(); }

		public: void RestoreSettingsToSaved() override { state.RestoreSettingsToSaved(); }
		
		public: void RestoreSettingsToDefault() override { state.RestoreSettingsToDefault(); }

		
		public: RasterizerSettings &SetFillModeSolid() override;
				
		public: RasterizerSettings &SetFillModeWireframe() override;
				
		public: RasterizerSettings &SetFrontIsCounterClockwise(bool value) override;

		public:RasterizerSettings &SetFrontfaceCulling() override;

		public:RasterizerSettings &SetBackfaceCulling() override;

		public:RasterizerSettings &SetNoCulling() override;			


		public: D3D12_RASTERIZER_DESC GetRasterizerDesc() const;

	};

	
}
