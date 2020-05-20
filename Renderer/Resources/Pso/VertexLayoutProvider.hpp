#pragma once
#include <d3d12.h>
#include "Resources/Pso/VertexLayoutTypes.hpp"
#include <vector>
#include <unordered_map>

namespace Renderer::DX12
{
	class VertexLayoutProvider
	{
		private: struct LayoutData
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
			
		};
		
		private: std::unordered_map<VertexLayoutTypes, LayoutData> layouts;


		
		public: VertexLayoutProvider();			
		
		protected: void AddLayout(VertexLayoutTypes type, LayoutData &&layout);
	
		
		public: D3D12_INPUT_LAYOUT_DESC GetLayoutDesc(VertexLayoutTypes layoutType) const;
				   			
	};

	
}
