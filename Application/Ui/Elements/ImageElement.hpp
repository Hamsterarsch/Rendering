#pragma once
#include "Ui/Core/UiElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"
#include "Core/ImageView.hpp"


namespace App::Ui
{	
	class ImageElement : public Core::UiElement
	{
		public: App::Core::ImageView image;
				 

		
		public: ImageElement(const App::Core::ImageView &image)
			:						
			image{ image }			
		{}

		
		public: void RenderAndQueryInternal(Core::UiBuilder &builder) override
		{
			builder.MakeImage(image);
						
		}
						
	};

	   	
}
