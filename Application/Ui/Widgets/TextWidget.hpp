#pragma once
#include "Ui/Core/WidgetBase.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include <string>

namespace App::Ui::Widgets
{
	class TextWidget : public WidgetBase
	{
		private: const char *text;



		public: TextWidget(const char *text) : text{ text } {}
		
		public: void RenderInternal(UiBuilder &builder) override
		{			
			builder.MakeText(text);
			
		}
		
	};

	
}
