#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Ui::Core
{
	class UiBuilder;
	
	class UiDecorator
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(UiDecorator)
		
		public: virtual void Decorate(UiBuilder &builder) = 0;
		
		public: virtual void Undecorate(UiBuilder &builder) {}
		
	};

	
}

