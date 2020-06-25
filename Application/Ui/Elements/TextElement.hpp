#pragma once
#include "Ui/Core/UiElement.hpp"
#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui
{
	class TextElement : public Core::UiElement
	{
		private: std::string text;



		public: TextElement(const char *text) : text{ text } {}

		public: TextElement(std::string &&text) : text{ std::move(text) } {}

		public: TextElement(const std::string &text) : text{ text } {}

		public: void SetText(const char *text) { this->text = text; }
		
		public: void RenderAndQueryInternal(Core::UiBuilder &builder) override
		{			
			builder.MakeText(text.c_str());
			
		}
		
	};

	
}
