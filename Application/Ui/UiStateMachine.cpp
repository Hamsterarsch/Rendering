#include "Ui/UiStateMachine.hpp"
#include "Ui/States/UiState.hpp"
#include "States/UiProjectFetchStartupState.hpp"


namespace App::Ui
{
	UiStateMachine::UiStateMachine() : stackLevelsHaveChanged{ false }
	{
		PushStateLevel(MakeUnique<States::UiProjectFetchStartupState>(*this));
		
		
	}


	
	UiStateMachine::~UiStateMachine() = default;


	
	void UiStateMachine::Update(Core::UiBuilder &builder)
	{
		stackLevelsHaveChanged = false;
		for(auto &&state : stateStack.front())
		{
			state.second->Update(builder);
			if(stackLevelsHaveChanged)
			{
				break;
			}
		}

		if(not stackLevelsHaveChanged)
		{
			for(auto &&stateToErase : statesToRemove)
			{
				stateStack.front().erase(stateToErase);
			}

			stateStack.front().merge(statesToAdd);

			//todo: add/remove anyway if levels were added or not ?
		}
		statesToRemove.clear();
		statesToAdd.clear();
		
	}


	
	void UiStateMachine::PopAllStateLevels(UniquePtr<States::UiState> &&newState)
	{
		stateStack.clear();
		PushStateLevel(std::move(newState));
				
	}
	
		void UiStateMachine::PushStateLevel(UniquePtr<States::UiState> &&state)
		{
			stateStack.emplace_front();
			
			auto *key{ state.get() };
			stateStack.front().insert( { key, std::move(state) } );

			stackLevelsHaveChanged = true;
			
		}



	void UiStateMachine::PopStateLevel()
	{
		stateStack.pop_front();

		stackLevelsHaveChanged = true;
		
	}



	void UiStateMachine::AddState(UniquePtr<States::UiState> &&state)
	{
		auto *key{ state.get() }; 
		statesToAdd.insert( {key, std::move(state)} );
		
	}



	void UiStateMachine::RemoveState(States::UiState &state)
	{		
		statesToRemove.emplace(&state);
		
	}

		
}
