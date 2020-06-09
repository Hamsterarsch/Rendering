#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Ui
{
	class StringInputTarget;
	
	class UiFrontend
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(UiFrontend)
		

		public: virtual void Update() = 0;
		
		public: virtual StringInputTarget *GetInputTargetString(size_t index) { return nullptr; }

		public: virtual bool *GetInputTargetBool(size_t index) { return nullptr; }
					
	};
		
	
}
