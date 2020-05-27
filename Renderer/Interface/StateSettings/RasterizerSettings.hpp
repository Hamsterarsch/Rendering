#pragma once
#include "StateSettings/StateSettings.hpp"


namespace Renderer
{
	class RasterizerSettings : public StateSettings
	{
		public: virtual void SetFillModeSolid() = 0;

		public: virtual void SetFillModeWireframe() = 0;


		public: virtual void SetFrontIsCounterClockwise(bool value) = 0;
						
	};
	
	
}