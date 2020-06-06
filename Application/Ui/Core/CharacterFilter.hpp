#pragma once
#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Ui
{
	class CharacterFilter
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CharacterFilter)

		public: virtual bool CharacterIsAllowed(char character) = 0;
				
	};
	

}
