#pragma once
#include "Ui/States/UiState.hpp"


namespace App::Ui::States
{
	class UiProjectStartupState final : public UiState
	{


		public: UiProjectStartupState(UiStateMachine &parent) : UiState{ parent } {}
		
		public: void Update(Core::UiBuilder &builder) override;
		
	};
	
	
}
