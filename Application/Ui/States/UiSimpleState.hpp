#pragma once
#include "Ui/States/UiState.hpp"
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui::States
{
	class UiSimpleState final : public UiState
	{
		private: UniquePtr<Core::UiFrontend> frontend;

		

		public: UiSimpleState(UiStateMachine &parent) : UiState{ parent } {}

				
		public: void Update(Core::UiBuilder &builder) override
		{
			if(frontend)
			{
				frontend->Update(builder);
			}
			
		}

		public: void SetFrontend(UniquePtr<Core::UiFrontend> &&frontend) { this->frontend = std::move(frontend); }
		
	};
	
	
}

