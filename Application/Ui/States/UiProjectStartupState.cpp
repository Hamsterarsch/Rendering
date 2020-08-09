#include "Ui/States/UiProjectStartupState.hpp"
#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui::States
{
	void UiProjectStartupState::Update(Core::UiBuilder &builder)
	{
		assetBrowser.Update(builder);
		sceneEditor.Update(builder);
		
	}
	
	
}
