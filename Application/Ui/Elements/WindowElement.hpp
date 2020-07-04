#pragma once
#include "Ui/Core/UiLayoutElementBasic.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"



namespace App::Ui
{
	class WindowElement final : public Core::UiLayoutElementBasic
	{
		private: std::string title;

		private: Core::UiFrontend *frontend;

		private: size_t isOpenTargetIndex;

		private: Math::Vector2 contentArea;

		public: UniquePtr<WindowElement> tabChild;

		public: bool isVerticalTabChild;

		public: float tabChildSizeInPercent;
		
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
			isVerticalTabChild{ false },
			tabChildSizeInPercent{ .5 },
			isNocollapse{ false },
			isStatic{ false }
		{
			size = position = pivot = { .5, .5 };
			
		}

		
		public: float GetContentWidth() const { return contentArea.x; }

		public: float GetContentHeight() const { return contentArea.y; }

		public: void SetTitle(const char *title) { this->title = title; }
		
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
											
			builder.DeclareName(title.c_str());
						
			if(tabChild)
			{
				builder.DeclareTabChildSize(tabChildSizeInPercent);
				if(isVerticalTabChild)
				{
					builder.DeclareTabChildDirectionDown();
				}
				
				builder.MakeTabWithChild(GetOpenTarget());				
			}
			else
			{
				builder.MakeTab(GetOpenTarget());
			}
						
		}

			private: bool *GetOpenTarget()
			{
				return frontend ? frontend->GetInputTargetBool(isOpenTargetIndex) : nullptr;
				
			}


		
		protected: void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override
		{			
			builder.LeaveWidget();

			if(tabChild)
			{
				tabChild->RenderAndQueryInput(builder);
			}
						
		}
				   					 				  		
	};

	
}
