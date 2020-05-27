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

	

	void BlendSettingsImpl::ResetToDefault()
	{
		current = saved = State();
	}

	

	void BlendSettingsImpl::SaveSettings()
	{
		saved = current;
		
	}



	void BlendSettingsImpl::RestoreSettings()
	{
		current = saved;
		
	}



	void BlendSettingsImpl::TargetSrc(const char *data)
	{
		current.srcBlend = *reinterpret_cast<const D3D12_BLEND *>(data);
		
	}



	void BlendSettingsImpl::TargetDst(const char *data)
	{
		current.dstBlend = *reinterpret_cast<const D3D12_BLEND *>(data);
		
	}



	void BlendSettingsImpl::TargetSrcAlpha(const char *data)
	{
		current.srcBlendAlpha = *reinterpret_cast<const D3D12_BLEND *>(data);
		
	}



	void BlendSettingsImpl::TargetDstAlpha(const char *data)
	{
		current.dstBlendAlpha = *reinterpret_cast<const D3D12_BLEND *>(data);
		
	}


	
	void BlendSettingsImpl::TargetBlendOpColor(const char *data)
	{
		current.blendOpColor = *reinterpret_cast<const D3D12_BLEND_OP *>(data);
		
	}


	
	void BlendSettingsImpl::TargetBlendOpAlpha(const char *data)
	{
		current.blendOpAlpha = *reinterpret_cast<const D3D12_BLEND_OP *>(data);
		
	}



	void BlendSettingsImpl::SetEnableBlend(const bool value)
	{
		current.enableBlend = value;
		
	}



	void BlendSettingsImpl::SetBlendZero(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_ZERO };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
	}



	void BlendSettingsImpl::SetBlendOne(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_ONE };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
	}



	void BlendSettingsImpl::SetBlendSrcColor(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_SRC_COLOR };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
	}



	void BlendSettingsImpl::SetBlendInverseSrcColor(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_INV_SRC_COLOR };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
	}



	void BlendSettingsImpl::SetBlendDstColor(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_DEST_COLOR };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
	}



	void BlendSettingsImpl::SetBlendInverseDstColor(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_INV_DEST_COLOR };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		
	}



	void BlendSettingsImpl::SetBlendSrcAlpha(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_SRC_ALPHA };
		(this->*target)(reinterpret_cast<const char *>(&value));
				
	}



	void BlendSettingsImpl::SetBlendInverseSrcAlpha(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_INV_SRC_ALPHA };
		(this->*target)(reinterpret_cast<const char *>(&value));
				
	}



	void BlendSettingsImpl::SetBlendDstAlpha(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_DEST_ALPHA };
		(this->*target)(reinterpret_cast<const char *>(&value));
				
	}



	void BlendSettingsImpl::SetBlendInverseDstAlpha(const t_targets_blend target)
	{
		const auto value{ D3D12_BLEND_INV_DEST_ALPHA };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
		
	}


	
	void BlendSettingsImpl::SetBlendOpAdd(const t_targets_op target)
	{
		const auto value{ D3D12_BLEND_OP_ADD };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
	}


	
	void BlendSettingsImpl::SetBlendOpSubtract(const t_targets_op target)
	{
		const auto value{ D3D12_BLEND_OP_SUBTRACT };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
	}


	
	void BlendSettingsImpl::SetBlendReversedSubtract(const t_targets_op target)
	{
		const auto value{ D3D12_BLEND_OP_REV_SUBTRACT };
		(this->*target)(reinterpret_cast<const char *>(&value));
		
	}



	D3D12_RENDER_TARGET_BLEND_DESC BlendSettingsImpl::GetBlendDesc() const
	{
		D3D12_RENDER_TARGET_BLEND_DESC desc{};
		desc.BlendEnable = current.enableBlend;
		desc.SrcBlend = current.srcBlend;
		desc.DestBlend = current.dstBlend;
		desc.SrcBlendAlpha = current.srcBlendAlpha;
		desc.DestBlendAlpha = current.dstBlendAlpha;
		desc.BlendOp = current.blendOpColor;
		desc.BlendOpAlpha = current.blendOpAlpha;
		desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		return desc;
		
	}

	
}
