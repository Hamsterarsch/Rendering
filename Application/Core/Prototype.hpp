#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/PtrTypes.hpp"


namespace App::Core
{
	template<class t_abstract>
	class Prototype
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Prototype)


		public: virtual UniquePtr<t_abstract> Clone() const = 0;
		
	};

	
}
