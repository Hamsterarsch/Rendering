#include "Resources/Pso/VertexLayoutProvider.hpp"


namespace Renderer
{
	namespace DX12
	{
		VertexLayoutProvider::VertexLayoutProvider()
		{
			{
				LayoutData layout;
				layout.elements.emplace_back
				(
					"POSITION",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					0,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
				);
				
				AddLayout(VertexLayoutTypes::PositionOnly, std::move(layout));
			}

			{
				LayoutData layout;
				layout.elements.emplace_back
				(
					"POSITION",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					0,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
				);

				layout.elements.emplace_back
				(
					"NORMAL",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					0,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
				);

				layout.elements.emplace_back
				(
					"TEXCOORD",
					0,
					DXGI_FORMAT_R32G32_FLOAT,
					0,
					0,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
				);
									
				AddLayout(VertexLayoutTypes::Standard, std::move(layout));
			}
				
		}


		
		D3D12_INPUT_LAYOUT_DESC VertexLayoutProvider::GetLayoutDesc(const VertexLayoutTypes layoutType) const
		{
			const auto &layout{ layouts.at(layoutType) };
			return { layout.elements.data(), layout.elements.size() };
				
		}


		
		void VertexLayoutProvider::AddLayout(const VertexLayoutTypes type, LayoutData &&layout)
		{
			layouts.insert( { type, std::move(layout) } );
				
		}

		
	}

	
}