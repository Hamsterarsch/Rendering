#pragma once
#include "Ui/Core/UiElement.hpp"
#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui
{
	class TextElement : public Core::UiElement
	{
		private: const char *text;



		public: TextElement(const char *text) : text{ text } {}

		public: void SetText(const char *text) { this->text = text; }
		
		public: void RenderAndQueryInternal(Core::UiBuilder &builder) override
		{			
			builder.MakeText(text);
			
		}
		
	};

	
}
