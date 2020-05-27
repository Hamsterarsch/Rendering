#pragma once
#include "StateSettings/StateSettings.hpp"


namespace Renderer
{
	class BlendSettingsBlendTargets
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(BlendSettingsBlendTargets)

		public: virtual void TargetSrc(const char *data) = 0;

		public: virtual void TargetDst(const char *data) = 0;
		
		public: virtual void TargetSrcAlpha(const char *data) = 0;

		public: virtual void TargetDstAlpha(const char *data) = 0;
		
	};

	class BlendSettingsBlendOpTargets
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(BlendSettingsBlendOpTargets)

		public: virtual void TargetBlendOpColor(const char *data) = 0;

		public: virtual void TargetBlendOpAlpha(const char *data) = 0;
		
		
	};

	class BlendSettingsTargets : public BlendSettingsBlendTargets, public BlendSettingsBlendOpTargets	{};
	
	class BlendSettings : public StateSettings, public BlendSettingsTargets
	{
		public: using t_targets_blend = void(BlendSettingsBlendTargets:: *)(const char *);
				
		public: using t_targets_op = void(BlendSettingsBlendOpTargets:: *)(const char *);
		

		public: virtual void SetEnableBlend(bool value) = 0;
		
		
		public: virtual void SetBlendZero(t_targets_blend target) = 0;

		public: virtual void SetBlendOne(t_targets_blend target) = 0;

		public: virtual void SetBlendSrcColor(t_targets_blend target) = 0;

		public: virtual void SetBlendInverseSrcColor(t_targets_blend target) = 0;

		public: virtual void SetBlendDstColor(t_targets_blend target) = 0;
		
		public: virtual void SetBlendInverseDstColor(t_targets_blend target) = 0;


		public: virtual void SetBlendSrcAlpha(t_targets_blend target) = 0;

		public: virtual void SetBlendInverseSrcAlpha(t_targets_blend target) = 0;

		public: virtual void SetBlendDstAlpha(t_targets_blend target) = 0;

		public: virtual void SetBlendInverseDstAlpha(t_targets_blend target) = 0;


		public: virtual void SetBlendOpAdd(t_targets_op target) = 0;

		public: virtual void SetBlendOpSubtract(t_targets_op target) = 0;

		public: virtual void SetBlendReversedSubtract(t_targets_op target) = 0;
						
	};

	
}
