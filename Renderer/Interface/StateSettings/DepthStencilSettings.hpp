#pragma once
#include "StateSettings/StateSettings.hpp"


namespace Renderer
{
	class DepthStencilSettings : public StateSettings
	{
		public: virtual void SetEnableDepth(bool value) = 0;
		
	};
	
	
}
