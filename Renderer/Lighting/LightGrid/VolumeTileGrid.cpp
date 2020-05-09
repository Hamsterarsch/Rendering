#include "Lighting/LightGrid/VolumeTileGrid.hpp"
#include "Math/Interface/Utility.hpp"

namespace Renderer
{
	VolumeTileGrid::VolumeTileGrid
	(
		const Math::VectorUint2 &approximateTileDimensions,
		const Math::Angle verticalFov,
		VolumeTileGridData &gridData
	)
		:
	fovTermForDepthCompute
		{
			2* Math::Tan(Radians(verticalFov) /2 ) / (gridData.screenDimensions.y / approximateTileDimensions.y)		
		},
	gridsize
		{
			gridData.screenDimensions.x / approximateTileDimensions.x,
			gridData.screenDimensions.y / approximateTileDimensions.y,
			Math::Log(gridData.farDistance/gridData.nearDistance) / Math::Log(1+fovTermForDepthCompute) +1
		},
	boundingBoxes(gridsize.x * gridsize.y * gridsize.z, Math::BoundingBox{})				
	{
		gridData.outFovTermForTileGridDepthCompute = fovTermForDepthCompute;
		gridData.outGridDimensions = gridsize;
		
	}


	
	Math::BoundingBox &VolumeTileGrid::GetTile(const unsigned x, const unsigned y,  const unsigned z)
	{
		return boundingBoxes[ gridsize.x + y*gridsize.x + z*gridsize.x*gridsize.y ];
			
	}	

	
}


