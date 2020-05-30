#pragma once
#include "StateSettings/RasterizerSettings.hpp"
#include <d3d12.h>


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
			
		} current, saved;

		
		public: void ResetToDefault() override;
		
		public: void SaveSettings() override;
		
		public: void RestoreSettings() override;

		
		public: RasterizerSettings &SetFillModeSolid() override;
				
		public: RasterizerSettings &SetFillModeWireframe() override;
				
		public: RasterizerSettings &SetFrontIsCounterClockwise(bool value) override;

		public:RasterizerSettings &SetFrontfaceCulling() override;

		public:RasterizerSettings &SetBackfaceCulling() override;

		public:RasterizerSettings &SetNoCulling() override;			


		public: D3D12_RASTERIZER_DESC GetRasterizerDesc() const;

	};

	
}
