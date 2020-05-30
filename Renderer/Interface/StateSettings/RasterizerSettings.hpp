#pragma once
#include "StateSettings/StateSettings.hpp"


namespace Renderer
{
	class RasterizerSettings : public StateSettings
	{
		public: virtual RasterizerSettings &SetFillModeSolid() = 0;

		public: virtual RasterizerSettings &SetFillModeWireframe() = 0;

		public: virtual RasterizerSettings &SetFrontfaceCulling() = 0;

		public: virtual RasterizerSettings &SetBackfaceCulling() = 0;

		public: virtual RasterizerSettings &SetNoCulling() = 0;


		public: virtual RasterizerSettings &SetFrontIsCounterClockwise(bool value) = 0;
						
	};
	
	
}