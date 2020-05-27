#pragma once
#include "StateSettings/StateSettings.hpp"


namespace Renderer
{
	class RasterizerSettings : public StateSettings
	{
		public: virtual void SetFillModeSolid() = 0;

		public: virtual void SetFillModeWireframe() = 0;

		public: virtual void SetFrontfaceCulling() = 0;

		public: virtual void SetBackfaceCulling() = 0;

		public: virtual void SetNoCulling() = 0;


		public: virtual void SetFrontIsCounterClockwise(bool value) = 0;
						
	};
	
	
}