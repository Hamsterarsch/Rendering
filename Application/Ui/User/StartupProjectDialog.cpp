#include "Ui/User/StartupProjectDialog.hpp"
#include "Ui/Core/UiElement.hpp"
#include "Ui/Elements/TextElement.hpp"
#include "Ui/Elements/ButtonElement.hpp"
#include "Ui/Elements/WindowElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "Windows/SelectPathDialog.hpp"
#include "AssetTypes/ProjectAsset.hpp"
#include "AssetSystem/Interface/AssetFileending.hpp"
#include "Core/Globals.hpp"
#include "Core/CreateProject.hpp"


namespace App::Ui::User
{
	StartupProjectDialogFrontend::StartupProjectDialogFrontend()
		:
			showCreateProjectDialog{ false },
			shouldOpenProject{ false },
			dialogCreateProject{ *this }
	{			
		auto grid{ MakeUnique<GridLayout>(2, 2) };

		grid +=
		{
			{0,0},
			Element<ButtonElement>(*this, 0, "Create New")
			->* Set{&ButtonElement::position, {.5,1}}
			->* Set{&ButtonElement::pivot, {.5,1}}
		};
		
		grid += 
		{
			{1,0},
			Element<ButtonElement>(*this, 1, "Open Project")
			->* Set{&ButtonElement::position, {.5,1}}
			->* Set{&ButtonElement::pivot, {.5,1}}
		};

		auto errorDisplay{ Element<TextElement>("No error") };
		errorDisplay->size = { .8, 0 };
		errorDisplay->pivot = { .5, 0 };
		errorDisplay->position = { .5, 0 };
		errorDisplay->SetIsHidden(true);
		this->errorDisplay = errorDisplay.get();
		
		grid += {{0,1, 2}, std::move(errorDisplay)};
					
		auto window{ Element<WindowElement>("Open a project or create a new one to begin") += std::move(grid) };					
		window->isNocollapse = true;
		window->isStatic = true;
		window->position.y = .4;
		window->size.y = .25;
		
		uiElements.push_front(std::move(window));
					
	}

	void StartupProjectDialogFrontend::Update(Core::UiBuilder &builder)
	{
		if(!RenderAndQueryInputForUiElements(builder))
		{
			return;
			
		}

		
		if(showCreateProjectDialog)
		{
			dialogCreateProject.OpenDialog();
		}

		if(shouldOpenProject)
		{			
			std::wstring projectAssetEnding{ Assets::ProjectAsset::GetAssetClassExtensionW() };
			projectAssetEnding += assetSystem::GetAssetFileendingW();

			const wchar_t *allowedExtensions[]{ projectAssetEnding.c_str() };
			
			const Windows::SelectPathDialog dialog{allowedExtensions, 1};
			const auto path{ dialog.GetSelectedItem() };
			if(!path.empty())
			{
				bool hasProjectVersionMismatch{ false };
				App::Core::globals.projectAssetSystem = App::Core::LoadProject(path, hasProjectVersionMismatch);

				if(hasProjectVersionMismatch)
				{
					errorDisplay->SetText("The selected project was created with a different version of the program. Please use the correct program version to open the project.");
					errorDisplay->SetIsHidden(false);
				}
				else
				{
					errorDisplay->SetIsHidden(true);
					uiElements.clear();
				}
				
			}
			
			
		}
		
		dialogCreateProject.Update(builder);
		
	}


	
	bool *StartupProjectDialogFrontend::GetInputTargetBool(const size_t index)
	{
		switch(index)
		{
			case 0: return &showCreateProjectDialog;
			case 1: return &shouldOpenProject;
			default: return nullptr;
		}
		
	}
	


	void StartupProjectDialogFrontend::NotifyAboutProjectCreation()
	{
		uiElements.clear();
		
	}

	
}
