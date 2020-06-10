#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Ui/Core/UiElement.hpp"
#include "Ui/User/CreateProjectDialog.hpp"
#include "Ui/Elements/EqualColumnsLayout.hpp"
#include "Ui/Elements/ButtonElement.hpp"
#include "Ui/Elements/WindowElement.hpp"


namespace App::Ui::User
{	
	class StartupProjectDialogFrontend final : public Core::UiFrontend
	{
		private: bool showCreateProjectDialog;

		private: bool showOpenProjectDialog;

		private: CreateProjectDialogFrontend dialogCreateProject;
		

		public: StartupProjectDialogFrontend()
			:
			showCreateProjectDialog{ false },
			showOpenProjectDialog{ false }
		{			
			auto grid{ MakeUnique<EqualColumnsLayout>() };

			auto btnCreateNew{ MakeUnique<ButtonElement>(*this, 0, "Create New") };
			btnCreateNew->alignment = .5;
			btnCreateNew->centerVertical = true;
			grid->AddChild(std::move(btnCreateNew));
			
			auto btnOpen{ MakeUnique<ButtonElement>(*this, 1, "Open Project") };
			btnOpen->alignment = .5;
			btnOpen->centerVertical = true;
			grid->AddChild(std::move(btnOpen));

			
			auto window{ MakeUnique<WindowElement>("Open a project or create a new one to begin") };					
			window->isNocollapse = true;
			window->isStatic = true;
			window->pos.y = .4;
			window->size.y = .25;
			
			window->AddChild(std::move(grid));

			uiElements.push_front(std::move(window));

			
		}
		

		
		public:	void Update(Core::UiBuilder &builder) override
		{
			if(!RenderAndQueryInputForUiElements(builder))
			{
				return;
				
			}

			
			if(showCreateProjectDialog)
			{
				dialogCreateProject.OpenDialog();
			}

			dialogCreateProject.Update(builder);
			
		}


		
		bool *GetInputTargetBool(size_t index) override
		{
			switch(index)
			{
				case 0: return &showCreateProjectDialog;
				case 1: return &showOpenProjectDialog;
				default: return nullptr;
			}
			
		}
		
	};
	
	
}
