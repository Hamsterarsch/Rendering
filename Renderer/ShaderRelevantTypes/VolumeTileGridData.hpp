#pragma once
#include "Math/Interface/Types/Matrix.hpp"

namespace Renderer
{
	struct VolumeTileGridData
	{
		Math::Matrix inverseProjection;
		
		Math::VectorUint3 outGridDimensions;
		
		float outFovTermForTileGridDepthCompute;
		
		Math::VectorUint2 screenDimensions;
		
		float nearDistance;
		
		float farDistance;
		
	};
	
	
}
