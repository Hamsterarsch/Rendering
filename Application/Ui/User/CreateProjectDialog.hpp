#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Ui/Misc/AssetNameFilter.hpp"
#include "Ui/User/StringInputTargetImpl.hpp"


namespace App::Ui
{
	class TextElement;
	class ButtonElement;
	
}

namespace App::Ui::States{ class UiCreateProjectState; }


namespace App::Ui::User
{		
	class CreateProjectDialogFrontend final : public Core::UiFrontend
	{		
		private: bool closeDialog;
		
		private: bool shouldSelectFolder;

		private: bool shouldCreateProject;

		private: bool hasValidFolder;

		private: States::UiCreateProjectState *parent;
		
		private: TextElement *errorDisplay;

		private: ButtonElement *confirmButton;
		
		public: StringInputTargetImpl<> selectedFolder;

		public: StringInputTargetImpl<Filter::AssetNameFilter> projectName;

				

		public: CreateProjectDialogFrontend(States::UiCreateProjectState &parent);
		

		public:	void Update(Core::UiBuilder &builder) override;
		
			private: bool SelectedFolderIsValid();
					 					 
			private: bool SelectedNameIsValid();
				
		bool *GetInputTargetBool(size_t index) override;
			   		 		
		Core::StringInputTarget *GetInputTargetString(size_t index) override;
		
		
	};
	
	
}
