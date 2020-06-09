#pragma once
#include "Ui/Core/UiElement.hpp"
#include "Shared/PtrTypes.hpp"


namespace App::Ui
{
	class UiFrontend;
	
	class UiAccessElement : public UiElement
	{
		private: SharedPtr<UiFrontend> frontend;

		
		
		public: UiAccessElement(const SharedPtr<UiFrontend> &frontend) : frontend{ frontend } {}

		protected: UiFrontend &GetFrontend() { return *frontend; }

		protected: const UiFrontend &GetFrontend() const { return *frontend; }
				   		
	};

	
}
