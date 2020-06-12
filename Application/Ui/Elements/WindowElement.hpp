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

		public: Math::Vector2 pivot;

		public: Math::Vector2 size;

		public: Math::Vector2 pos;

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
			pivot{ .5, .5 },
			size{ .5, .5 },
			pos{ .5, .5 },
			isNocollapse{ false },
			isStatic{ false }
		{}

		
		
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
			.DeclareAlignment(.5)
			.DeclareSize(size.x, size.y)
			.DeclareTabPos(pos, pivot)						
			.MakeTab(isOpenTarget);
			
		}


		
		protected: void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override
		{
			builder.LeaveWidget();
			
		}
		
	};

	
}
