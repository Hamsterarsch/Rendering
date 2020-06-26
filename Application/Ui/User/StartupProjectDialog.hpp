#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Ui/User/CreateProjectDialog.hpp"


namespace App::Ui{ class TextElement; }

namespace App::Ui::States{ class UiProjectFetchStartupState; }


namespace App::Ui::User
{	
	class StartupProjectDialogFrontend final : public Core::UiFrontend
	{
		private: bool showCreateProjectDialog;

		private: bool shouldOpenProject;

		private: TextElement *errorDisplay;

		private: States::UiProjectFetchStartupState *parent;
		
		//private: CreateProjectDialogFrontend dialogCreateProject;

		

		public: StartupProjectDialogFrontend(States::UiProjectFetchStartupState &parent);
		
		
		public:	void Update(Core::UiBuilder &builder) override;
						
		bool *GetInputTargetBool(size_t index) override;
			
		
	};
	
	
}
