#pragma once
#include "Ui/Core/WidgetBase.hpp"
#include "Ui/Core/WidgetBuilder.hpp"
#include <string>

namespace App::Ui::Widgets
{
	class TextWidget : public WidgetBase
	{
		private: const char *text;



		public: TextWidget(const char *text) : text{ text } {}
		
		public: void RenderInternal(WidgetBuilder &builder) override
		{			
			builder.MakeText(text);
			
		}
		
	};

	
}
