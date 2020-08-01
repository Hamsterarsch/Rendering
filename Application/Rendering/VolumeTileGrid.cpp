#include "Rendering/VolumeTileGrid.hpp"
#include "Math/Interface/Utility.hpp"


namespace App::Rendering
{
	VolumeTileGrid::VolumeTileGrid
	(
		const Math::VectorUint2 &approximateTileDimensions,
		const Math::Angle verticalFov,
		const float tileDepthScale,
		VolumeTileGridData &gridData
	)
		:
	fovTermForDepthCompute
		{
			2* Math::Tan(Radians(verticalFov) /2 ) * tileDepthScale / (gridData.screenDimensions.y / approximateTileDimensions.y)		
		},
	gridsize
		{
			gridData.screenDimensions.x / approximateTileDimensions.x,
			gridData.screenDimensions.y / approximateTileDimensions.y,
			std::ceil(Math::Log2(gridData.farDistance-gridData.nearDistance) / Math::Log2(1+fovTermForDepthCompute))
		},
	boundingBoxes(gridsize.x * gridsize.y * gridsize.z, RenderingBoundingBox{})				
	{
		gridData.outFovTermForTileGridDepthCompute = fovTermForDepthCompute;
		gridData.outGridDimensions = gridsize;
		
	}


	
	RenderingBoundingBox &VolumeTileGrid::GetTile(const unsigned x, const unsigned y,  const unsigned z)
	{
		return boundingBoxes[ gridsize.x + y*gridsize.x + z*gridsize.x*gridsize.y ];
			
	}	

	
}



