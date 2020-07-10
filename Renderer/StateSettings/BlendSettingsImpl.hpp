#pragma once
#include "StateSettings/BlendSettings.hpp"
#include <d3d12.h>
#include "StateImpl.hpp"


namespace Renderer::DX12
{
	class BlendSettingsImpl final : public BlendSettings
	{
		private: struct State
		{
			bool enableBlend;
			D3D12_BLEND srcBlend;
			D3D12_BLEND dstBlend;
			D3D12_BLEND srcBlendAlpha;
			D3D12_BLEND dstBlendAlpha;
			D3D12_BLEND_OP blendOpColor;
			D3D12_BLEND_OP blendOpAlpha;
			
			State();
			
		};

		private: StateImpl<State> state;

		
		
		public:	void ResetAllToDefault() override { state.ResetAllToDefault(); }

		public: void SaveSettings() override { state.SaveSettings(); }

		public: void RestoreSettingsToSaved() override { state.RestoreSettingsToSaved(); }
		
		public: void RestoreSettingsToDefault() override { state.RestoreSettingsToDefault(); }
		
		
		public: void TargetSrc(const char* data) override;

		public: void TargetDst(const char* data) override;

		public: void TargetSrcAlpha(const char* data) override;

		public: void TargetDstAlpha(const char* data) override;

		public: void TargetBlendOpColor(const char *data) override;				  

		public: void TargetBlendOpAlpha(const char *data) override;
		
		
		public: BlendSettings &SetEnableBlend(bool value) override;

		public: BlendSettings &SetBlendZero(t_targets_blend target) override;
				
		public: BlendSettings &SetBlendOne(t_targets_blend target) override;

		public: BlendSettings &SetBlendSrcColor(t_targets_blend target) override;

		public: BlendSettings &SetBlendInverseSrcColor(t_targets_blend target) override;

		public: BlendSettings &SetBlendDstColor(t_targets_blend target) override;

		public: BlendSettings &SetBlendInverseDstColor(t_targets_blend target) override;

		
		public: BlendSettings &SetBlendSrcAlpha(t_targets_blend target) override;

		public: BlendSettings &SetBlendInverseSrcAlpha(t_targets_blend target) override;

		public: BlendSettings &SetBlendDstAlpha(t_targets_blend target) override;

		public: BlendSettings &SetBlendInverseDstAlpha(t_targets_blend target) override;

		
		public: BlendSettings &SetBlendOpAdd(t_targets_op target) override;				  

		public: BlendSettings &SetBlendOpSubtract(t_targets_op target) override;			  

		public: BlendSettings &SetBlendReversedSubtract(t_targets_op target) override;

		
		public: D3D12_RENDER_TARGET_BLEND_DESC GetBlendDesc() const;

	};
	
	
}
