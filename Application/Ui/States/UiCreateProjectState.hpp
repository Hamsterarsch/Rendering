#pragma once
#include "Ui/States/UiState.hpp"
#include "Ui/User/CreateProjectDialog.hpp"


namespace App::Ui::States
{
	class UiCreateProjectState final : public UiState
	{
		private: User::CreateProjectDialogFrontend ui;


		public: UiCreateProjectState(UiStateMachine &parent) : UiState{ parent }, ui{ *this } {}

		
		public: void Update(Core::UiBuilder &builder) override;

		public: void NotifyProjectCreated();

		public: void NotifyCreationAborted();
		
	};
	
	
}
