#include "StateSettings/VertexLayoutSettingsImpl.hpp"
#include "StateSettings/FormatTargetsImpl.hpp"
#include "StateSettings/SemanticTargetsImpl.hpp"


namespace Renderer::DX12
{
	void VertexLayoutSettingsImpl::ResetToDefault()
	{
		current = saved = State{};
		
	}


	
	void VertexLayoutSettingsImpl::SaveSettings()
	{
		saved = current;
		
	}


	
	void VertexLayoutSettingsImpl::RestoreSettings()
	{
		current = saved;
		
	}


	
	VertexLayoutSettings &VertexLayoutSettingsImpl::AddLayoutElementDesc
	(
		const Semantic semantic,
		const unsigned char semanticIndex,
		const Format format,
		const unsigned byteOffset
	)
	{
		auto *semanticValue{ reinterpret_cast<const char *>((SemanticTargetsImpl::Get().*semantic)()) };
		auto *formatValue{ reinterpret_cast<const DXGI_FORMAT *>((FormatTargetsImpl::Get().*format)()) };

		current.elements.emplace_back( D3D12_INPUT_ELEMENT_DESC
		{
			semanticValue,
			semanticIndex,
			*formatValue,
			0,
			byteOffset,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		});

		return *this;
		
	}


	
	D3D12_INPUT_LAYOUT_DESC VertexLayoutSettingsImpl::GetLayoutDesc() const
	{
		D3D12_INPUT_LAYOUT_DESC desc{};
		desc.pInputElementDescs = current.elements.data();
		desc.NumElements = current.elements.size();

		return desc;
		
	}

	
}
