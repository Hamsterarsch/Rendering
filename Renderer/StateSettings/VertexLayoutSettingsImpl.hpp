#pragma once
#include "StateSettings/VertexLayoutSettings.hpp"
#include <d3d12.h>
#include <vector>


namespace Renderer::DX12
{
	class VertexLayoutSettingsImpl final : public VertexLayoutSettings
	{
		private: struct State
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
						
		}current, saved;

		
		
		public: void ResetToDefault() override;
		
		public: void SaveSettings() override;
				
		public: void RestoreSettings() override;
		
				
		public: VertexLayoutSettings &AddLayoutElementDesc(Semantic semantic, unsigned char semanticIndex, Format format, unsigned byteOffset) override;

		public: D3D12_INPUT_LAYOUT_DESC GetLayoutDesc() const;
				
	};
	
	
}
