#pragma once
#include "Ui/Core/UiLayoutElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui
{
	class ModalElement : public Core::UiLayoutElement
	{
		private: const char *title;
		
		public: Math::Vector2 size;

		


		public: ModalElement(const char *title)
			:
			title{ title },
			size{ .3, .3 }
		 {
		 	SetIsHidden(true);
			
		 }

		

		protected: void OnPreRenderAndQueryChildren(Core::UiBuilder &builder) override
		{						
			builder
			.DeclareName(title)
			.DeclareAlignment(.5)
			.DeclareSize(size.x, size.y)								
			.MakeModal();

		}

		
		
		void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override
		{
			builder.LeaveWidget();
			
		}
		
	};

	
}
