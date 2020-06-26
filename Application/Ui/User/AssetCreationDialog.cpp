#include "Ui/User/AssetCreationDialog.hpp"
#include "Core/Application.hpp"
#include "Ui/Elements/ModalElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"


namespace App::Ui::User
{
	AssetCreationDialogFrontend::AssetCreationDialogFrontend(App::Core::Application &app)
		:
		app{ &app }
	{
		//Element<ModalElement>("Select Asset")
		
	}


	
	void AssetCreationDialogFrontend::Update(Core::UiBuilder &builder)
	{
		RenderAndQueryInputForUiElements(builder);
				
	}

	
}
