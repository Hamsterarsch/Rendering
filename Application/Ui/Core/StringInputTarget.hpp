#pragma once
#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Ui
{
	class CharacterFilter;
	
	class StringInputTarget
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(StringInputTarget)

		
		public: virtual void Resize(size_t toLength) = 0;
		
		public: virtual void GetCapacity() const = 0;

		public: virtual char *GetBuffer() = 0;

		public: virtual CharacterFilter GetFilter() const = 0;
		
	};
	

}