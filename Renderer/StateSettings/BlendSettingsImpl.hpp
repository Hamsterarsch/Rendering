#pragma once
#include "StateSettings/BlendSettings.hpp"
#include <d3d12.h>


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
			
		} current, saved;

		
		
		public:	void ResetToDefault() override;

		public: void SaveSettings() override;

		public: void RestoreSettings() override;

		
		public: void TargetSrc(const char* data) override;

		public: void TargetDst(const char* data) override;

		public: void TargetSrcAlpha(const char* data) override;

		public: void TargetDstAlpha(const char* data) override;

		public: void TargetBlendOpColor(const char *data) override;				  

		public: void TargetBlendOpAlpha(const char *data) override;
		
		
		public: void SetEnableBlend(bool value) override;

		public: void SetBlendZero(t_targets_all target) override;
				
		public: void SetBlendOne(t_targets_all target) override;

		public: void SetBlendSrcColor(t_targets_color target) override;

		public: void SetBlendInverseSrcColor(t_targets_color target) override;

		public: void SetBlendDstColor(t_targets_color target) override;

		public: void SetBlendInverseDstColor(t_targets_color target) override;

		
		public: void SetBlendSrcAlpha(t_targets_alpha target) override;

		public: void SetBlendInverseSrcAlpha(t_targets_alpha target) override;

		public: void SetBlendDstAlpha(t_targets_alpha target) override;

		public: void SetBlendInverseDstAlpha(t_targets_alpha target) override;

		
		public: void SetBlendOpAdd(t_targets_op target) override;				  

		public: void SetBlendOpSubtract(t_targets_op target) override;			  

		public: void SetBlendReversedSubtract(t_targets_op target) override;

		
		public: D3D12_RENDER_TARGET_BLEND_DESC GetBlendDesc() const;

	};
	
	
}
