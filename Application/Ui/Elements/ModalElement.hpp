#pragma once
#include "Ui/Core/UiLayoutElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui
{
	class ModalElement : public Core::UiLayoutElement
	{
		private: const char *title;

		


		public: ModalElement(const char *title)
			: 
			title{ title }			
		{
		 	SetIsHidden(true);
			size = { .3, .3 };
			pivot = { .5, .5 };
			
		}

		

		protected: void OnPreRenderAndQueryChildren(Core::UiBuilder &builder) override
		{						
			builder
			.DeclareName(title)			
			.MakeModal();

		}

		
		
		void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override
		{
			builder.LeaveWidget();
			
		}
		
	};

	
}
