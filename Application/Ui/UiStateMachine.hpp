#pragma once
#include "Shared/PtrTypes.hpp"
#include <forward_list>
#include <unordered_map>
#include <unordered_set>


namespace App::Core{ class Application; }


namespace App::Ui
{
	namespace States{ class UiState; }
	namespace Core{ class UiBuilder; }

		
	class UiStateMachine
	{
		private: std::forward_list<std::unordered_map<States::UiState *, UniquePtr<States::UiState>>> stateStack;

		private: std::unordered_map<States::UiState *, UniquePtr<States::UiState>> statesToAdd;

		private: std::unordered_set<States::UiState *> statesToRemove;
		
		private: bool stackLevelsHaveChanged;

		private: App::Core::Application *app;		
		

		
		public: UiStateMachine(App::Core::Application &app);

		public: ~UiStateMachine();
		
		public: UiStateMachine(UiStateMachine &&) = default;

		public: UiStateMachine &operator=(UiStateMachine &&) = default;
		
		public: UiStateMachine(const UiStateMachine &) = delete;

		public: UiStateMachine &operator=(const UiStateMachine &) = delete;
		
		
		public: App::Core::Application &GetApp() { return *app; }
		
		public: void Update(Core::UiBuilder &builder);


		public: void PopAllStateLevels(UniquePtr<States::UiState> &&newState);
		
		public: void PushStateLevel(UniquePtr<States::UiState> &&state);
		
		public: void PopStateLevel();

		public: void AddState(UniquePtr<States::UiState> &&state);

		public: void RemoveState(States::UiState &state);


						
	};

	
}
