#pragma once
#include "Math/Interface/Types/Matrix.hpp"


namespace App::Rendering
{
	struct VolumeTileGridData
	{
		Math::Matrix inverseProjection;
		
		Math::VectorUint3 outGridDimensions;
		
		float outFovTermForTileGridDepthCompute;
		
		Math::Vector2 screenDimensions;
		
		float nearDistance;
		
		float farDistance;
		
	};
	
	
}