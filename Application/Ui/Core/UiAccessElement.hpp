#pragma once
#include "Ui/Core/UiElement.hpp"
#include "Shared/PtrTypes.hpp"


namespace App::Ui::Core
{
	class UiFrontend;
	
	class UiAccessElement : public UiElement
	{
		private: UiFrontend *frontend;

		
		
		public: UiAccessElement(UiFrontend &frontend) : frontend{ &frontend } {}

		protected: UiFrontend &GetFrontend() { return *frontend; }

		protected: const UiFrontend &GetFrontend() const { return *frontend; }
				   						   		
	};

	
}
