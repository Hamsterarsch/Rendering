#include "Ui/UiStateMachine.hpp"
#include "Ui/States/UiState.hpp"


namespace App::Ui
{
	UiStateMachine::UiStateMachine(Windows::Application &app) : app{ &app }
	{
	}


	
	UiStateMachine::~UiStateMachine() = default;


	
	void UiStateMachine::Update(Core::UiBuilder &builder)
	{
		if(not currentState)
		{
			return;
			
		}
			   		
		if(auto newState{ currentState->Update(builder) })
		{
			currentState = std::move(newState);
		}
		
	}

	
}
