#pragma once
#include "Shared/PtrTypes.hpp"


namespace App::Windows{ class Application; }


namespace App::Ui
{
	namespace States{ class UiState; }
	namespace Core{ class UiBuilder; }
	
	
	class UiStateMachine
	{
		private: UniquePtr<States::UiState> currentState;

		private: Windows::Application *app;


		
		public: UiStateMachine(Windows::Application &app);

		public: ~UiStateMachine();
		
		public: UiStateMachine(UiStateMachine &&) = default;

		public: UiStateMachine &operator=(UiStateMachine &&) = default;
		
		public: UiStateMachine(const UiStateMachine &) = delete;

		public: UiStateMachine &operator=(const UiStateMachine &) = delete;
		
		
		public: void Update(Core::UiBuilder &builder);
		
	};

	
}
