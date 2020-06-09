#pragma once
#include "Ui/Core/WidgetBase.hpp"
#include "Ui/Core/WidgetBehavior.hpp"

#include "Ui/Core/UiBuilder.hpp"
#include <string>
#include <functional>
#include "Shared/PtrTypes.hpp"

namespace App::Ui::Widgets
{
	class ExecuteCallbackOnTrueBehavior final : public WidgetBehavior
	{
		private: bool wasClicked;
		
		public: std::function<void()> callback;

		
		public: ExecuteCallbackOnTrueBehavior(const std::function<void()> &callback) : wasClicked{ false }, callback{ callback } {}
		
		public: bool *GetInputTargetBool(size_t index) override{ return &wasClicked; }
		
		void Update() override
		{
			if(wasClicked && callback)
			{
				callback();				
			}
			
		}
		
	};
	
	class ToggleWidgetHideStateBehavior : public WidgetBehavior
	{
		WidgetBase *targetWidget;

		bool toggleWidget;
		
		public: ToggleWidgetHideStateBehavior(WidgetBase &targetWidget) : targetWidget{ &targetWidget }, toggleWidget{ false } {}
		
		public: void Update() override
		{
			if(toggleWidget)
			{
				targetWidget->SetIsHidden(!targetWidget->IsHidden());				
			}
			
		}

		public: bool* GetInputTargetBool(size_t index) override { return &toggleWidget; }
		
			
	};
	
	class ButtonWidget : public WidgetBase
	{
		private: const char *name;

		private: UniquePtr<WidgetBehavior> ownedBehavior;

		private: WidgetBehavior *parentBehavior;

		private: unsigned targetBehaviorIndex;
		
		public: float alignment;

		public: bool centerVertical;
		
		
		public: ButtonWidget(const char *name, UniquePtr<WidgetBehavior> &&behavior, unsigned targetBehaviorIndex = 0)
			:
			name{ name },
			ownedBehavior{ std::move(behavior) },
			parentBehavior{ nullptr },
			targetBehaviorIndex{ targetBehaviorIndex },
			alignment{ 0 },
			centerVertical{ false }
		{}

		public: ButtonWidget(const char *name, WidgetBehavior &parentBehavior, unsigned targetBehaviorIndex = 0)
			:
			name{ name },
			parentBehavior{ &parentBehavior },
			targetBehaviorIndex{ targetBehaviorIndex },
			alignment{ 0 },
			centerVertical{ false }
		{}
		
		public: void RenderInternal(UiBuilder &builder) override
		{
			auto *targetBehavior{  parentBehavior ? parentBehavior : ownedBehavior.get() };
			
			builder.DeclareName(name)
			.DeclareAlignment(alignment)			
			.MakeButton(targetBehavior->GetInputTargetBool(targetBehaviorIndex), centerVertical);
						
		}

		WidgetBehavior *GetBehavior() override
		{
			return ownedBehavior.get();
			
		}
		
	};



	
}
