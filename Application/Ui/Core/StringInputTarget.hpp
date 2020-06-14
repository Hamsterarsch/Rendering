#pragma once
#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Ui::Core
{
	class StringInputTarget
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(StringInputTarget)

		
		public: virtual void Resize(size_t toLength) = 0;
		
		public: virtual size_t GetCapacity() const = 0;

		public: virtual char *GetBuffer() = 0;

		public: virtual bool CharacterIsForbidden(char character) const = 0;

		public: virtual bool IsReadOnly() const = 0;

		public: virtual void NotifyContentChanged() = 0;

		public: virtual bool ContentWasChanged() const = 0;

		public: virtual void ClearChangedState() = 0;
		
	};
	

}