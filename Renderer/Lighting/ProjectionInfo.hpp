#pragma once
#include "Math/Interface/Types/Matrix.hpp"

namespace Renderer
{
	struct ProjectionInfo
	{
		Math::Matrix inverseProjection;
		
		Math::VectorUint3 gridDimensions;
		
		float fovTermForTileGridDepthCompute;
		
		Math::Vector2 screenDimensions;
		
		float nearDistance;
		
		float farDistance;
		
	};
	
	
}
