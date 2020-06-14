#pragma once
#include "Ui/Core/UiLayoutElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"



namespace App::Ui
{
	class WindowElement final : public Core::UiLayoutElement
	{
		private: const char *title;

		private: Core::UiFrontend *frontend;

		private: size_t isOpenTargetIndex;

		public: bool isNocollapse;

		public: bool isStatic;


		
		public: WindowElement(Core::UiFrontend &frontend, size_t isOpenTargetIndex, const char *title)	: WindowElement{ title }			
		{
			this->frontend = &frontend;
			this->isOpenTargetIndex = isOpenTargetIndex;
			
		}
		
		public: WindowElement(const char *title) 
			:		
			title{ title },
			frontend{ nullptr },
			isOpenTargetIndex{ 0 },			
			isNocollapse{ false },
			isStatic{ false }
		{
			size = position = pivot = { .5, .5 };
			
		}

		
		
		protected: void OnPreRenderAndQueryChildren(Core::UiBuilder &builder) override
		{
			if(isNocollapse)
			{
				builder.DeclareTabNocollapse();
			}

			if(isStatic)
			{
				builder.DeclareTabStatic();
			}

			bool *isOpenTarget{ frontend ? frontend->GetInputTargetBool(isOpenTargetIndex) : nullptr };
			
			builder
			.DeclareName(title)							
			.MakeTab(isOpenTarget);
			
		}


		
		protected: void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override
		{
			builder.LeaveWidget();
			
		}
		
	};

	
}
