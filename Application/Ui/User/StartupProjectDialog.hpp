#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Ui/Core/UiElement.hpp"
#include "Ui/User/CreateProjectDialog.hpp"
#include "Ui/Elements/EqualColumnsLayout.hpp"
#include "Ui/Elements/ButtonElement.hpp"
#include "Ui/Elements/WindowElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"


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

			grid += Element<ButtonElement>(*this, 0, "Create New")
					->* Set{&ButtonElement::position, {.5,.5}}
					->* Set{&ButtonElement::pivot, {.5,.5}};
			
			grid += Element<ButtonElement>(*this, 1, "Open Project")
					->* Set{&ButtonElement::position, {.5,.5}}
					->* Set{&ButtonElement::pivot, {.5,.5}};
						
			auto window{ Element<WindowElement>("Open a project or create a new one to begin") += std::move(grid) };					
			window->isNocollapse = true;
			window->isStatic = true;
			window->position.y = .4;
			window->size.y = .25;
			
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
