#pragma once
#include "Ui/Core/UiFrontend.hpp"


namespace App::Core{ class Application; }


namespace App::Ui::User
{
	class AssetCreationDialogFrontend final : public Core::UiFrontend
	{
		private: App::Core::Application *app;
		
		
		AssetCreationDialogFrontend(App::Core::Application &app);
		
		public: void Update(Core::UiBuilder& builder) override;
		
	};


}
