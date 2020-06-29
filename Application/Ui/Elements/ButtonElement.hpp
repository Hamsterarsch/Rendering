#pragma once
#include "Ui/Core/UiAccessElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui
{	
	class ButtonElement : public Core::UiAccessElement
	{
		protected: const char *name;

		protected: size_t isClickedTargetIndex;

		public: bool isDisabled;
				 

		
		public: ButtonElement(Core::UiFrontend &frontend, size_t isClickedTargetIndex, const char *name)
			:
			UiAccessElement{ frontend },
			name{ name },
			isClickedTargetIndex{ isClickedTargetIndex },
			isDisabled{ false }
		{}

		
		public: void RenderAndQueryInternal(Core::UiBuilder &builder) override
		{
			if(isDisabled)
			{
				builder.DeclareButtonDisabled();
			}
			
			builder
			.DeclareName(name)
			.MakeButton(GetFrontend().GetInputTargetBool(isClickedTargetIndex));
						
		}
						
	};


	class ToggleButtonElement final : public ButtonElement
	{
		private: bool isToggledOn;
		
		

		public: ToggleButtonElement(Core::UiFrontend &frontend, size_t isClickedTargetIndex, const char *name)
			:
			ButtonElement{ frontend, isClickedTargetIndex, name },
			isToggledOn{ false }
		{}

							   				 
		void RenderAndQueryInternal(Core::UiBuilder &builder) override
		{
			if(isDisabled)
			{
				builder.DeclareButtonDisabled();
			}

			bool isPressed;			
			builder
			.DeclareName(name)
			.MakeButton(&isPressed);

			isToggledOn ^= isPressed;
			if(auto *out{ GetFrontend().GetInputTargetBool(isClickedTargetIndex)})
			{
				*out = isToggledOn;
			}
			
		}
		
	};
	
	   	
}
