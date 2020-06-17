#pragma once
#include "Ui/Core/UiAccessElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"
#include "Core/ImageView.hpp"


namespace App::Ui
{	
	class ButtonElement : public Core::UiAccessElement
	{
		private: size_t isClickedTargetIndex;

		public: App::Core::ImageView image;
		
		public: bool isDisabled;
				 

		
		public: ButtonElement(Core::UiFrontend &frontend, size_t isClickedTargetIndex, const App::Core::ImageView &image)
			:
			UiAccessElement{ frontend },			
			isClickedTargetIndex{ isClickedTargetIndex },
			image{ image },
			isDisabled{ false }
		{}

		
		public: void RenderAndQueryInternal(Core::UiBuilder &builder) override
		{
			if(isDisabled)
			{
				builder.DeclareButtonDisabled();
			}
			
			builder.MakeImageButton(image, GetFrontend().GetInputTargetBool(isClickedTargetIndex));
						
		}
						
	};

	   	
}
