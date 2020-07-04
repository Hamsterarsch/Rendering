#pragma once
#include "Ui/Core/UiLayoutElementBasic.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui
{
	class ModalElement : public Core::UiLayoutElementBasic
	{
		private: const char *title;

		public: float padding;
		


		public: ModalElement(const char *title)
			: 
			title{ title },
			padding{}
		{		 	
			size = { .3, .3 };
			pivot = { .5, .5 };
			
		}

		

		protected: void OnPreRenderAndQueryChildren(Core::UiBuilder &builder) override
		{						
			builder
			.DeclareName(title)
			.DeclarePadding(padding)
			.MakeModal();

		}

		
		
		void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override
		{
			builder.LeaveWidget();
			
		}
		
	};

	
}
