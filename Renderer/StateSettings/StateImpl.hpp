#pragma once
#include "StateSettings/StateSettings.hpp"


namespace Renderer::DX12
{
	template<class t_state>
	class StateImpl final : public StateSettings
	{
		public: t_state saved;
		
		public: t_state current;


		
		public: void ResetAllToDefault() override
		{
			saved = current = t_state();
			
		}
		
		public: void SaveSettings() override
		{
			saved = current;
			
		}
		
		public: void RestoreSettingsToSaved() override
		{
			current = saved;
			
		}
		
		public: void RestoreSettingsToDefault() override
		{
			current = t_state();
			
		}
				
	};
	
	
}
