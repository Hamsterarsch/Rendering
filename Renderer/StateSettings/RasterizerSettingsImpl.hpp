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

		
		public: void SetFillModeSolid() override;
				
		public: void SetFillModeWireframe() override;
				
		public: void SetFrontIsCounterClockwise(bool value) override;

		public:void SetFrontfaceCulling() override;

		public:void SetBackfaceCulling() override;

		public:void SetNoCulling() override;			


		public: D3D12_RASTERIZER_DESC GetRasterizerDesc() const;

	};

	
}
