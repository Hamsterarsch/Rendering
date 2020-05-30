#include "StateSettings/RasterizerSettingsImpl.hpp"


namespace Renderer::DX12
{
	RasterizerSettingsImpl::State::State() :
		frontIsCounterClockwise{ true },
		fillMode{ D3D12_FILL_MODE_SOLID },
		cullMode{ D3D12_CULL_MODE_BACK }
	{
	}


	
	void RasterizerSettingsImpl::ResetToDefault()
	{
		current = saved = State{};
		
	}


	
	void RasterizerSettingsImpl::SaveSettings()
	{
		saved = current;
		
	}


	
	void RasterizerSettingsImpl::RestoreSettings()
	{
		current = saved;
		
	}


		
	RasterizerSettings &RasterizerSettingsImpl::SetFillModeSolid()
	{
		current.fillMode = D3D12_FILL_MODE_SOLID;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetFillModeWireframe()
	{
		current.fillMode = D3D12_FILL_MODE_WIREFRAME;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetFrontIsCounterClockwise(const bool value)
	{
		current.frontIsCounterClockwise = value;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetFrontfaceCulling()
	{
		current.cullMode = D3D12_CULL_MODE_FRONT;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetBackfaceCulling()
	{
		current.cullMode = D3D12_CULL_MODE_BACK;
		
		return *this;

	}


	
	RasterizerSettings &RasterizerSettingsImpl::SetNoCulling()
	{
		current.cullMode = D3D12_CULL_MODE_NONE;
		
		return *this;

	}



	D3D12_RASTERIZER_DESC RasterizerSettingsImpl::GetRasterizerDesc() const
	{
		D3D12_RASTERIZER_DESC desc{};

		desc.FillMode = current.fillMode;
		desc.FrontCounterClockwise = current.frontIsCounterClockwise;
		desc.CullMode = current.cullMode;

		desc.DepthClipEnable = true;

		return desc;
		
	}

	
}
