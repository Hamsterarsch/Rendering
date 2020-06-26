#pragma once
#include "Shared/PtrTypes.hpp"
#include "Shared/InterfaceHelpers.hpp"


namespace App::Ui{ class UiStateMachine; }

namespace App::Ui::Core{ class UiBuilder; }


namespace App::Ui::States
{	
	class UiState
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS_NOCTOR(UiState)

		protected: UiStateMachine *parent;


		
		public: UiState(UiStateMachine &parent) : parent{ &parent } {}

		public: UiStateMachine &GetParent() { return *parent; }

		
		public: virtual void Update(Core::UiBuilder &builder) = 0;
		
	};

	
}
