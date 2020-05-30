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
		

		public: virtual BlendSettings &SetEnableBlend(bool value) = 0;
		
		
		public: virtual BlendSettings &SetBlendZero(t_targets_blend target) = 0;

		public: virtual BlendSettings &SetBlendOne(t_targets_blend target) = 0;

		public: virtual BlendSettings &SetBlendSrcColor(t_targets_blend target) = 0;

		public: virtual BlendSettings &SetBlendInverseSrcColor(t_targets_blend target) = 0;

		public: virtual BlendSettings &SetBlendDstColor(t_targets_blend target) = 0;
		
		public: virtual BlendSettings &SetBlendInverseDstColor(t_targets_blend target) = 0;


		public: virtual BlendSettings &SetBlendSrcAlpha(t_targets_blend target) = 0;

		public: virtual BlendSettings &SetBlendInverseSrcAlpha(t_targets_blend target) = 0;

		public: virtual BlendSettings &SetBlendDstAlpha(t_targets_blend target) = 0;

		public: virtual BlendSettings &SetBlendInverseDstAlpha(t_targets_blend target) = 0;


		public: virtual BlendSettings &SetBlendOpAdd(t_targets_op target) = 0;

		public: virtual BlendSettings &SetBlendOpSubtract(t_targets_op target) = 0;

		public: virtual BlendSettings &SetBlendReversedSubtract(t_targets_op target) = 0;
						
	};

	
}
