#pragma once
#include "Ui/Core/WidgetBase.hpp"
#include "Ui/Core/WidgetBehavior.hpp"

#include "Ui/Core/WidgetBuilder.hpp"
#include <string>
#include <functional>
#include "Shared/PtrTypes.hpp"

namespace App::Ui::Widgets
{
	class ToggleWidgetHideStateBehavior : public WidgetBehavior
	{
		WidgetBase *targetWidget;

		bool hideWidget;
		
		public: ToggleWidgetHideStateBehavior(WidgetBase &targetWidget) : targetWidget{ &targetWidget }, hideWidget{ false } {}
		
		public: void Update() override
		{
			if(hideWidget)
			{
				targetWidget->SetIsHidden(true);				
			}
			
		}

		public: bool* GetInputTargetBool(size_t index) override { return &hideWidget; }
		
			
	};
	
	class ButtonWidget : public WidgetBase
	{
		private: std::string name;

		private: UniquePtr<WidgetBehavior> behavior;

		public: float alignment;

		public: bool centerVertical;
		
		
		public: ButtonWidget(const char *name, UniquePtr<WidgetBehavior> &&behavior) : name{ name }, behavior{ std::move(behavior) }, alignment{ 0 }, centerVertical{ false } {}
		
		public: void RenderInternal(WidgetBuilder &builder) override
		{			
			builder.DeclareName(name.c_str())
			.DeclareAlignment(alignment)			
			.MakeButton(behavior->GetInputTargetBool(0), centerVertical);
			
			

		}

		WidgetBehavior *GetBehavior() override{ return behavior.get(); }
		
	};



	
}
