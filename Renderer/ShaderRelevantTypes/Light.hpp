#pragma once
#include "Math/Interface/Types/Matrix.hpp"


namespace Renderer
{
	struct Light
	{
		public: Math::Vector3 worldPos;
		
		public: float radius;

		public: Math::Vector3 worldForwardVector;

		public: float innerBlendRadians;

		public: Math::Vector3 color;

		public: float outerBlendRadians;		

	};
	
	
}
