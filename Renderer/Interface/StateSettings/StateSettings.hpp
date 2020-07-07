#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class StateSettings
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(StateSettings)


		public: virtual void ResetAllToDefault() = 0;

		public: virtual void SaveSettings() = 0;

		public: virtual void RestoreSettingsToSaved() = 0;

		public: virtual void RestoreSettingsToDefault() = 0;
		
	};
		
	
}
