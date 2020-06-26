#pragma once
#include "Ui/States/UiState.hpp"
#include <vector>
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui::States
{
	class UiSimpleState final : public UiState
	{
		private: std::vector<UniquePtr<Core::UiFrontend>> frontends;

		

		public: UiSimpleState(UiStateMachine &parent) : UiState{ parent } {}

				
		public: void Update(Core::UiBuilder &builder) override
		{
			for(auto &&frontend : frontends)
			{
				frontend->Update(builder);
			}
			
		}

		public: void AddFrontend(UniquePtr<Core::UiFrontend> &&frontend) { frontends.emplace_back(std::move(frontend)); }
		
	};
	
	
}

