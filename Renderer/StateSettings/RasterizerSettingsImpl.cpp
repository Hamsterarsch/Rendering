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


		
	void RasterizerSettingsImpl::SetFillModeSolid()
	{
		current.fillMode = D3D12_FILL_MODE_SOLID;
		
	}


	
	void RasterizerSettingsImpl::SetFillModeWireframe()
	{
		current.fillMode = D3D12_FILL_MODE_WIREFRAME;
		
	}


	
	void RasterizerSettingsImpl::SetFrontIsCounterClockwise(const bool value)
	{
		current.frontIsCounterClockwise = value;
		
	}


	
	void RasterizerSettingsImpl::SetFrontfaceCulling()
	{
		current.cullMode = D3D12_CULL_MODE_FRONT;
		
	}


	
	void RasterizerSettingsImpl::SetBackfaceCulling()
	{
		current.cullMode = D3D12_CULL_MODE_BACK;
		
	}


	
	void RasterizerSettingsImpl::SetNoCulling()
	{
		current.cullMode = D3D12_CULL_MODE_NONE;
		
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
