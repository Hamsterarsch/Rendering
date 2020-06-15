#include "Ui/User/CreateProjectDialog.hpp"
#include "Windows/SelectPathDialog.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Elements/ButtonElement.hpp"
#include "Ui/Elements/InputElement.hpp"
#include "Ui/Elements/TextElement.hpp"
#include "Ui/Elements/ModalElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include <filesystem>
#include "Core/Globals.hpp"
#include "Core/CreateProject.hpp"
#include "Ui/States/UiCreateProjectState.hpp"


namespace App::Ui::User
{
	CreateProjectDialogFrontend::CreateProjectDialogFrontend(States::UiCreateProjectState &parent)
		:
		closeDialog{ false },
		shouldSelectFolder{ false },
		shouldCreateProject{ false },
		hasValidFolder{ false },
		parent{ &parent },
		selectedFolder{ true }		
	{
		auto grid{ Element<GridLayout>(5, 3, 4) };			
		
		grid += 
		{
			{0,0, 2}, Element<ButtonElement>(*this, 1, "Select Folder")				
			->* Set{&ButtonElement::position, {.5,.5}}
			->* Set{&ButtonElement::pivot, {.5,.5}}
		};
		
		grid += 
		{
			{2,0, 3}, Element<InputElement<Core::StringInputTarget>>(*this, 0, "Folder Display")
				->* Set{&Core::UiElement::size, {1, 0}}
				->* Set{&Core::UiElement::position, {0,.5}}
				->* Set{&Core::UiElement::pivot, {0,.5}}
		};
		
		grid += 
		{
			{0,1, 2}, Element<TextElement>("Project Name")
				->* Set{&TextElement::position, {.5,.5}}					
				->* Set{&TextElement::pivot, {.5, .5}}
		};

		grid += 
		{
			{2,1, 3}, Element<InputElement<Core::StringInputTarget>>(*this, 1, "NameInput")
				->* Set{&Core::UiElement::size, {1, 0}}
				->* Set{&Core::UiElement::position, {0,.5}}
				->* Set{&Core::UiElement::pivot, {0,.5}}
		};

		auto errorElement{ Element<TextElement>("No error") };
		errorElement->size = { 0, 1 };
		errorElement->pivot = { .5, 0 };
		errorElement->position = { .5, 0 };
		errorElement->SetIsHidden(true);
		errorDisplay = errorElement.get();

		auto confirmButton{ Element<ButtonElement>(*this, 2, "Confirm") };
		confirmButton->size = { 1, 0 };
		confirmButton->pivot =  { 0, 1 };
		confirmButton->position = { 0, 1 };
		confirmButton->isDisabled = true;
		this->confirmButton = confirmButton.get();
		
		grid +=	
		{
			{0,2, 5},
			Element<GridLayout>(2, 2, 3) += {{0, 1}, std::move(confirmButton)}
				+=
				{
					{1, 1}, Element<ButtonElement>(*this, 0, "Abort")
					->* Set{&ButtonElement::size, {1, 0} }
					->* Set{&ButtonElement::pivot, {0,1}}
					->* Set{&ButtonElement::position, {0,1}}
				}										
				+= {{0, 0, 2}, std::move(errorElement)}
		};
								
		uiElements.push_front(Element<ModalElement>("Create a new Project")  ->* Set{&ModalElement::size, {.5, .3}} += std::move(grid));							
		uiElements.front()->SetIsHidden(false);
		
	}


	
	void CreateProjectDialogFrontend::Update(Core::UiBuilder &builder)
	{
		if(closeDialog)
		{				
			parent->NotifyCreationAborted();
			return;
			
		}

		
		if(!RenderAndQueryInputForUiElements(builder))
		{
			return;
		}
					
		
		if(shouldSelectFolder)
		{
			const Windows::SelectPathDialog dialog{};				
			selectedFolder.data = dialog.GetSelectedItem().string();

			hasValidFolder = SelectedFolderIsValid();
			if(hasValidFolder)
			{
				if(SelectedNameIsValid())
				{
					confirmButton->isDisabled = false;											
				}
			}
			else
			{
				selectedFolder.data.clear();
			}					
		}

		
		if(projectName.ContentWasChanged())
		{
			if(hasValidFolder && SelectedNameIsValid())
			{
				confirmButton->isDisabled = false;
			}				
		}

		
		if(shouldCreateProject)
		{
			App::Core::globals.projectAssetSystem = App::Core::CreateProject(projectName.data.c_str(), selectedFolder.data.c_str());

			parent->NotifyProjectCreated();
			return;
			
		}
		
	}

		bool CreateProjectDialogFrontend::SelectedFolderIsValid()
		{
			if(selectedFolder.data.empty())
			{
				return false;
				
			}
		
			std::filesystem::path path{  selectedFolder.data };

			for(auto &&entry : std::filesystem::directory_iterator{ path })
			{
				errorDisplay->SetText("The selected folder is not empty !");
				errorDisplay->SetIsHidden(false);
				
				return false;
				
			}

			errorDisplay->SetIsHidden(true);
			return true;				
			
		}

		bool CreateProjectDialogFrontend::SelectedNameIsValid()
		{
			return not projectName.data.empty();
			
		}


	
	bool *CreateProjectDialogFrontend::GetInputTargetBool(const size_t index)
	{
		switch(index)
		{
			case 0:	return &closeDialog;
			case 1: return &shouldSelectFolder;
			case 2: return &shouldCreateProject;
			default: return nullptr;				
		}
		
	}


	
	Core::StringInputTarget *CreateProjectDialogFrontend::GetInputTargetString(const size_t index)
	{
		if(index == 0)
		{
			return &selectedFolder;
		}

		return &projectName;
		
	}

	
}

