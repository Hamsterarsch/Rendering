#include "Ui/States/UiProjectFetchStartupState.hpp"
#include "Ui/States/UiCreateProjectState.hpp"
#include "Ui/States/UiProjectStartupState.hpp"
#include "Ui/UiStateMachine.hpp"


namespace App::Ui::States
{
	void UiProjectFetchStartupState::Update(Core::UiBuilder &builder)
	{
		ui.Update(builder);
		
	}

	void UiProjectFetchStartupState::NotifyProjectOpened(const char *projectAssetPath)
	{
		parent->PopAllStateLevels(MakeUnique<UiProjectStartupState>(*parent, projectAssetPath));
		
	}

	void UiProjectFetchStartupState::NotifyOpenCreateProjectDialog()
	{
		parent->PushStateLevel(MakeUnique<UiCreateProjectState>(*parent));
		
	}
	
}
