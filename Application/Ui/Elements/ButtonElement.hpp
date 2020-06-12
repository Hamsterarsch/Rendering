#pragma once
#include "Ui/Core/UiAccessElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui
{	
	class ButtonElement : public Core::UiAccessElement
	{
		private: const char *name;

		private: size_t isClickedTargetIndex;
				 

		
		public: ButtonElement(Core::UiFrontend &frontend, size_t isClickedTargetIndex, const char *name)
			:
			UiAccessElement{ frontend },
			name{ name },
			isClickedTargetIndex{ isClickedTargetIndex }			
		{}

		
		public: void RenderAndQueryInternal(Core::UiBuilder &builder) override
		{
			builder
			.DeclareName(name)
			.MakeButton(GetFrontend().GetInputTargetBool(isClickedTargetIndex));
						
		}
						
	};

	   	
}
