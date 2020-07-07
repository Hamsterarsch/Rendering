#include "StateSettings/RasterizerSettingsImpl.hpp"


namespace Renderer::DX12
{
	RasterizerSettingsImpl::State::State() :
		frontIsCounterClockwise{ true },
		fillMode{ D3D12_FILL_MODE_SOLID },
		cullMode{ D3D12_CULL_MODE_BACK }
	{
	}


		
	RasterizerSettings &RasterizerSettingsImpl::SetFillModeSolid()
	{
		state.current.fillMode = D3D12_FILL_MODE_SOLID;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetFillModeWireframe()
	{
		state.current.fillMode = D3D12_FILL_MODE_WIREFRAME;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetFrontIsCounterClockwise(const bool value)
	{
		state.current.frontIsCounterClockwise = value;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetFrontfaceCulling()
	{
		state.current.cullMode = D3D12_CULL_MODE_FRONT;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetBackfaceCulling()
	{
		state.current.cullMode = D3D12_CULL_MODE_BACK;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetNoCulling()
	{
		state.current.cullMode = D3D12_CULL_MODE_NONE;
		
		return *this;

	}



	D3D12_RASTERIZER_DESC RasterizerSettingsImpl::GetRasterizerDesc() const
	{
		D3D12_RASTERIZER_DESC desc{};

		desc.FillMode = state.current.fillMode;
		desc.FrontCounterClockwise = state.current.frontIsCounterClockwise;
		desc.CullMode = state.current.cullMode;

		desc.DepthClipEnable = true;

		return desc;
		
	}

	
}
