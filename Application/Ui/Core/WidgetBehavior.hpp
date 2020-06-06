#pragma once
#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Ui
{
	class StringInputTarget;
	
	
	class WidgetBehavior
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(WidgetBehavior)

		public: virtual StringInputTarget *GetInputTargetString(size_t index) { return nullptr; }

		public: virtual bool *GetInputTargetBool(size_t index) { return nullptr; }

		public: virtual void Update() = 0;
		
		
	};
	

}