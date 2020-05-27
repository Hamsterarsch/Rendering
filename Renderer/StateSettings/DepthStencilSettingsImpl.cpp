#include "StateSettings/DepthStencilSettingsImpl.hpp"


namespace Renderer::DX12
{
	DepthStencilSettingsImpl::State::State() :
		enableDepth{ true },
		depthFunc{ D3D12_COMPARISON_FUNC_LESS_EQUAL }
	{
	}
	

	void DepthStencilSettingsImpl::ResetToDefault()
	{
		saved = current = State{};
		
	}

	

	void DepthStencilSettingsImpl::SaveSettings()
	{
		saved = current;
		
	}
	

	
	void DepthStencilSettingsImpl::RestoreSettings()
	{
		current = saved;
		
	}


	
	void DepthStencilSettingsImpl::SetEnableDepth(const bool value)
	{
		current.enableDepth = value;
		
	}


	
	D3D12_DEPTH_STENCIL_DESC DepthStencilSettingsImpl::GetDepthStencilDesc() const
	{
		D3D12_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = current.enableDepth;
		desc.DepthFunc = current.depthFunc;
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		desc.StencilEnable = false;
		desc.BackFace = desc.FrontFace = MakeDefaultStencilDesc();

		return desc;
		
	}

		constexpr D3D12_DEPTH_STENCILOP_DESC DepthStencilSettingsImpl::MakeDefaultStencilDesc()
		{
			return { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
		
		}

	
}
