#pragma once
#include "Ui/Core/WidgetBase.hpp"


#include "Ui/Core/UiBuilder.hpp"
#include <string>

namespace App::Ui::Widgets
{
	class WindowWidget : public WidgetBase
	{
		private: const char *title;

		public: Math::Vector2 pivot;

		public: Math::Vector2 size;

		public: Math::Vector2 pos;

		public: bool isNocollapse;

		public: bool isStatic;
		
		
		public: WindowWidget(const char *title) : title{ title }, pivot{ .5, .5 }, size{ .5, .5 }, pos{ .5, .5 }, isNocollapse{ false }, isStatic{ false } {}
		
		public: void RenderInternal(UiBuilder &builder) override
		{
			if(isNocollapse)
			{
				builder.DeclareTabNocollapse();
			}

			if(isStatic)
			{
				builder.DeclareTabStatic();
			}
			
			builder
			.DeclareName(title)
			.DeclareAlignment(.5)
			.DeclareTabSize(size)
			.DeclareTabPos(pos, pivot)						
			.MakeTab();

			RenderChildren(builder);
			builder.LeaveWidget();
			
		}

		WidgetBehavior* GetBehavior() override{ return nullptr; }
		
	};

	
}
