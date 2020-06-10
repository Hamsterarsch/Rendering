#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Ui/Core/StringInputTarget.hpp"
#include "Ui/Misc/AssetNameFilter.hpp"
#include "Windows/SelectPathDialog.hpp"
#include <string>
#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Elements/ButtonElement.hpp"
#include "Ui/Elements/InputElement.hpp"
#include "Ui/Elements/TextElement.hpp"
#include "Ui/Elements/ModalElement.hpp"


namespace App::Ui::User
{
	struct PassAllFilter
	{
		static bool CharacterIsForbidden(char character) { return false; }
	};
	
	template<class t_filter = PassAllFilter>
	class StringInputTargetImpl final : public Core::StringInputTarget
	{
		public: std::string data;

		public: bool isReadOnly{ false };


		public: StringInputTargetImpl(bool isReadOnly = false) : isReadOnly{ isReadOnly } {}
		
		public:	void Resize(size_t toLength) override{ data.resize(toLength); }

		public: size_t GetCapacity() const override{ return data.capacity(); }

		public: char* GetBuffer() override { return data.data(); }

		public: bool CharacterIsForbidden(char character) const override { return t_filter::CharacterIsForbidden(character); }

		public: bool IsReadOnly() const override { return isReadOnly; }

	};
	


	
	class CreateProjectDialogFrontend final : public Core::UiFrontend
	{
		private: bool closeDialog;
		
		private: bool shouldSelectFolder;

		private: bool shouldCreateProject;
				 		
		public: StringInputTargetImpl<> selectedFolder;

		public: StringInputTargetImpl<Filter::AssetNameFilter> projectName;



		public: CreateProjectDialogFrontend()
			:
			closeDialog{ false },
			shouldSelectFolder{ false },
			shouldCreateProject{ false },
			selectedFolder{ true }
		{
			auto grid{ MakeUnique<GridLayout>(5, 3) };

			grid->DeclareChildPos(0,0, 2)
			.AddChild(MakeUnique<ButtonElement>(*this, 1, "Select Folder"));

			grid->DeclareChildPos(2, 0, 3)
			.AddChild(MakeUnique<InputElement<Core::StringInputTarget>>(*this, 0, "Folder Display"));
			
			grid->DeclareChildPos(0, 1, 2)
			.AddChild(MakeUnique<TextElement>("Project Name"));

			grid->DeclareChildPos(2, 1, 3)
			.AddChild(MakeUnique<InputElement<Core::StringInputTarget>>(*this, 1, "NameInput"));

			auto subGrid{ MakeUnique<GridLayout>(2, 1) };

			subGrid->DeclareChildPos(1,0)
			.AddChild(MakeUnique<ButtonElement>(*this, 0, "Abort"));

			grid->DeclareChildPos(0, 2, 5)
			.AddChild(std::move(subGrid));

			auto createProjectDialog{ MakeUnique<ModalElement>("Create a new Project") };
			createProjectDialog->AddChild(std::move(grid));
			
			uiElements.push_front(std::move(createProjectDialog));
			
		}

		public: void OpenDialog()
		{
			closeDialog = false;
			uiElements.front()->SetIsHidden(false);
			
		}

		
		public:	void Update(Core::UiBuilder &builder) override
		{
			if(closeDialog)
			{				
				uiElements.front()->SetIsHidden(true);
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
			}
			
		}


		
		bool *GetInputTargetBool(size_t index) override
		{
			switch(index)
			{
				case 0:	return &closeDialog;
				case 1: return &shouldSelectFolder;
				case 2: return &shouldCreateProject;
				default: return nullptr;				
			}
			
		}


		
		Core::StringInputTarget *GetInputTargetString(size_t index) override
		{
			if(index == 0)
			{
				return &selectedFolder;
			}

			return &projectName;
			
		}
		
	};
	
	
}
