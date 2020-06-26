#pragma once
#include "Ui/States/UiState.hpp"
#include "Ui/User/StartupProjectDialog.hpp"


namespace App::Ui::States
{
	class UiProjectFetchStartupState final : public UiState
	{
		private: User::StartupProjectDialogFrontend ui;


		public: UiProjectFetchStartupState(UiStateMachine &parent) : UiState{ parent }, ui{ *this } {}

		
		public: void Update(Core::UiBuilder &builder) override;

		public: void NotifyProjectOpened(const char *projectAssetPath);

		public: void NotifyOpenCreateProjectDialog();
		
	};
	
	
}
