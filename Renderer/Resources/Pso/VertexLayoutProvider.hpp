#pragma once
#include <d3d12.h>
#include "Resources/Pso/VertexLayoutTypes.hpp"
#include <vector>
#include <unordered_map>

namespace Renderer
{
	namespace DX12
	{		
		class VertexLayoutProvider
		{
			private: struct LayoutData
			{
				std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
				
			};
			
			private: std::unordered_map<VertexLayoutTypes, LayoutData> layouts;


			
			public: VertexLayoutProvider();			

			public: VertexLayoutProvider(const VertexLayoutProvider &) = default;

			public: VertexLayoutProvider(VertexLayoutProvider &&) = default;

			public: VertexLayoutProvider &operator=(const VertexLayoutProvider &) = default;

			public: VertexLayoutProvider &operator=(VertexLayoutProvider &&) = default;
						
			public: virtual ~VertexLayoutProvider() = default;
			
			
			public: D3D12_INPUT_LAYOUT_DESC GetLayoutDesc(VertexLayoutTypes layoutType) const;


			protected: void AddLayout(VertexLayoutTypes type, LayoutData &&layout);


			
		};

		
	}

	
}
