#include "Resources/Pso/VertexLayoutProvider.hpp"


namespace Renderer::DX12
{
	VertexLayoutProvider::VertexLayoutProvider()
	{
		D3D12_INPUT_ELEMENT_DESC descPosition
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};
						
		AddLayout(VertexLayoutTypes::Position, {{descPosition}});


		D3D12_INPUT_ELEMENT_DESC descNormal
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};
		
		AddLayout(VertexLayoutTypes::PositionNormal, {{descPosition, descNormal}});


		D3D12_INPUT_ELEMENT_DESC descTexcoord
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};

		AddLayout(VertexLayoutTypes::PositionNormalTexcoord, {{descPosition, descNormal, descTexcoord}});
			
	}

		void VertexLayoutProvider::AddLayout(const VertexLayoutTypes type, LayoutData &&layout)
		{
			layouts.insert( { type, std::move(layout) } );
				
		}


	
	D3D12_INPUT_LAYOUT_DESC VertexLayoutProvider::GetLayoutDesc(const VertexLayoutTypes layoutType) const
	{
		const auto &layout{ layouts.at(layoutType) };
		return { layout.elements.data(), layout.elements.size() };
			
	}

	
}