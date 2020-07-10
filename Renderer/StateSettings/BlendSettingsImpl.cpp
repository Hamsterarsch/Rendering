#include "StateSettings/BlendSettingsImpl.hpp"


namespace Renderer::DX12
{
	BlendSettingsImpl::State::State() :
		enableBlend{ false },
		srcBlend{ D3D12_BLEND_ONE },
		dstBlend{ D3D12_BLEND_ZERO },
		srcBlendAlpha{ D3D12_BLEND_ONE },
		dstBlendAlpha{ D3D12_BLEND_ZERO },
		blendOpColor{ D3D12_BLEND_OP_ADD },
		blendOpAlpha{ D3D12_BLEND_OP_ADD }
	{
	}

	

	void BlendSettingsImpl::TargetSrc(const char *data)
	{
		state.current.srcBlend = *reinterpret_cast<const D3D12_BLEND *>(data);
		
	}



	void BlendSettingsImpl::TargetDst(const char *data)
	{
		state.current.dstBlend = *reinterpret_cast<const D3D12_BLEND *>(data);
		
	}



	void BlendSettingsImpl::TargetSrcAlpha(const char *data)
	{
		state.current.srcBlendAlpha = *reinterpret_cast<const D3D12_BLEND *>(data);
		
	}



	void BlendSettingsImpl::TargetDstAlpha(const char *data)
	{
		state.current.dstBlendAlpha = *reinterpret_cast<const D3D12_BLEND *>(data);
		
	}


	
	void BlendSettingsImpl::TargetBlendOpColor(const char *data)
	{
		state.current.blendOpColor = *reinterpret_cast<const D3D12_BLEND_OP *>(data);
		
	}


	
	void BlendSettingsImpl::TargetBlendOpAlpha(const char *data)
	{
		state.current.blendOpAlpha = *reinterpret_cast<const D3D12_BLEND_OP *>(data);
		
	}



	BlendSettings &BlendSettingsImpl::SetEnableBlend(const bool value)
	{
		state.current.enableBlend = value;

		return *this;
		
	}



	BlendSettings &BlendSettingsImpl::SetBlendZero(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_ZERO };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
	}



	BlendSettings &BlendSettingsImpl::SetBlendOne(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_ONE };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
	}



	BlendSettings &BlendSettingsImpl::SetBlendSrcColor(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_SRC_COLOR };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
	}



	BlendSettings &BlendSettingsImpl::SetBlendInverseSrcColor(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_INV_SRC_COLOR };
		(this->*target)(reinterpret_cast<const char *>(&value));

		return *this;
		
	}



	BlendSettings &BlendSettingsImpl::SetBlendDstColor(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_DEST_COLOR };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
	}



	BlendSettings &BlendSettingsImpl::SetBlendInverseDstColor(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_INV_DEST_COLOR };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
		
	}



	BlendSettings &BlendSettingsImpl::SetBlendSrcAlpha(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_SRC_ALPHA };
		(this->*target)(reinterpret_cast<const char *>(&value));
				
		return *this;
	}



	BlendSettings &BlendSettingsImpl::SetBlendInverseSrcAlpha(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_INV_SRC_ALPHA };
		(this->*target)(reinterpret_cast<const char *>(&value));
				
		return *this;
	}



	BlendSettings &BlendSettingsImpl::SetBlendDstAlpha(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_DEST_ALPHA };
		(this->*target)(reinterpret_cast<const char *>(&value));
				
		return *this;
	}



	BlendSettings &BlendSettingsImpl::SetBlendInverseDstAlpha(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_INV_DEST_ALPHA };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
		
	}


	
	BlendSettings &BlendSettingsImpl::SetBlendOpAdd(const t_targets_op target)
	{
		const auto value{ D3D12_BLEND_OP_ADD };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
	}


	
	BlendSettings &BlendSettingsImpl::SetBlendOpSubtract(const t_targets_op target)
	{
		const auto value{ D3D12_BLEND_OP_SUBTRACT };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
	}


	
	BlendSettings &BlendSettingsImpl::SetBlendReversedSubtract(const t_targets_op target)
	{
		const auto value{ D3D12_BLEND_OP_REV_SUBTRACT };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		return *this;
	}



	D3D12_RENDER_TARGET_BLEND_DESC BlendSettingsImpl::GetBlendDesc() const
	{
		D3D12_RENDER_TARGET_BLEND_DESC desc{};
		desc.BlendEnable = state.current.enableBlend;
		desc.SrcBlend = state.current.srcBlend;
		desc.DestBlend = state.current.dstBlend;
		desc.SrcBlendAlpha = state.current.srcBlendAlpha;
		desc.DestBlendAlpha = state.current.dstBlendAlpha;
		desc.BlendOp = state.current.blendOpColor;
		desc.BlendOpAlpha = state.current.blendOpAlpha;
		desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		return desc;
		
	}

	
}
