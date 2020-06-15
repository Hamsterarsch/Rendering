#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Ui/User/CreateProjectDialog.hpp"


namespace App::Ui{ class TextElement; }

namespace App::Windows{ class Application; }


namespace App::Ui::User
{	
	class StartupProjectDialogFrontend final : public Core::UiFrontend
	{
		private: bool showCreateProjectDialog;

		private: bool shouldOpenProject;

		private: TextElement *errorDisplay;

		private: Windows::Application *app;
		
		private: CreateProjectDialogFrontend dialogCreateProject;

		

		public: StartupProjectDialogFrontend(Windows::Application &app);
		
		
		public:	void Update(Core::UiBuilder &builder) override;
						
		bool *GetInputTargetBool(size_t index) override;
			
		
	};
	
	
}
