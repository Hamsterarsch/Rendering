#pragma once
#include "Ui/Core/WidgetBase.hpp"
#include "Ui/Core/WidgetBuilder.hpp"
#include <string>

namespace App::Ui::Widgets
{
	class ModalWidget : public WidgetBase
	{
		private: const char *title;

		public: Math::Vector2 size;

		private: UniquePtr<WidgetBehavior> behavior;


		public: ModalWidget(const char *title, UniquePtr<WidgetBehavior> behavior = {}) : title{ title }, size{ .3, .3 }, behavior{ std::move(behavior) } { SetIsHidden(true); }
		
		public: void RenderInternal(WidgetBuilder &builder) override
		{
			
			builder
			.DeclareName(title)
			.DeclareAlignment(.5)
			.DeclareTabSize(size)								
			.MakeModal(nullptr);

			RenderChildren(builder);
			builder.LeaveWidget();
			
		}

		public: WidgetBehavior *GetBehavior() override { return behavior.get(); }
		
	};

	
}
