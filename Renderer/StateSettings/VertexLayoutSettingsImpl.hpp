#pragma once
#include "StateSettings/VertexLayoutSettings.hpp"
#include <d3d12.h>
#include <vector>
#include "StateImpl.hpp"


namespace Renderer::DX12
{
	class VertexLayoutSettingsImpl final : public VertexLayoutSettings
	{
		private: struct State
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
						
		};

		private: StateImpl<State> state;
		
		
		public: void ResetAllToDefault() override { state.ResetAllToDefault(); }
		
		public: void SaveSettings() override { state.SaveSettings(); }
				
		public: void RestoreSettingsToSaved() override { state.RestoreSettingsToSaved(); }
		
		public: void RestoreSettingsToDefault() override { state.RestoreSettingsToDefault(); }
		
		
				
		public: VertexLayoutSettings &AddLayoutElementDesc(Semantic semantic, unsigned char semanticIndex, Format format, unsigned byteOffset) override;

		public: D3D12_INPUT_LAYOUT_DESC GetLayoutDesc() const;

	};
	
	
}
