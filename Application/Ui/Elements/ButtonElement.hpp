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

		public: float alignment;

		public: bool centerVertical;
		

		
		public: ButtonElement(Core::UiFrontend &frontend, size_t isClickedTargetIndex, const char *name)
			:
			UiAccessElement{ frontend },
			name{ name },
			isClickedTargetIndex{ isClickedTargetIndex },
			alignment{ 0 },
			centerVertical{ false }
		{}

		
		public: void RenderAndQueryInternal(Core::UiBuilder &builder) override
		{
			builder.DeclareName(name)
			.DeclareAlignment(alignment)			
			.MakeButton(GetFrontend().GetInputTargetBool(isClickedTargetIndex), centerVertical);
						
		}
						
	};

	   	
}
