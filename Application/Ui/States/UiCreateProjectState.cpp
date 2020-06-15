#include "Ui/States/UiCreateProjectState.hpp"
#include "Ui/UiStateMachine.hpp"
#include "UiProjectStartupState.hpp"


namespace App::Ui::States
{
	void UiCreateProjectState::Update(Core::UiBuilder &builder)
	{
		ui.Update(builder);
		
	}



	void UiCreateProjectState::NotifyProjectCreated()
	{
		parent->PopAllStateLevels(MakeUnique<UiProjectStartupState>(*parent));
		
	}



	void UiCreateProjectState::NotifyCreationAborted()
	{
		parent->PopStateLevel();
		
	}

	
}
