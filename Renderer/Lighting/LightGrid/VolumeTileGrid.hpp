#pragma once
#include "Math/Interface/Types/BoundingBox.hpp"
#include "Math/Interface/Types/Angle.hpp"
#include "ShaderRelevantTypes/VolumeTileGridData.hpp"
#include <vector>


namespace Renderer
{
	class VolumeTileGrid
	{
		private: float fovTermForDepthCompute;
		
		private: Math::VectorUint3 gridsize;

		private: std::vector<Math::BoundingBox> boundingBoxes;
			


		public: VolumeTileGrid() = default;
		
		public: VolumeTileGrid(const Math::VectorUint2 &approximateTileDimensions, Math::Angle verticalFov, VolumeTileGridData &gridData);


		public: size_t SizeInBytes() const { return sizeof(decltype(boundingBoxes)::value_type) * boundingBoxes.size(); }
		
		public: void *GetData() { return boundingBoxes.data(); }

		public: size_t GetTileStride() const { return sizeof(decltype(boundingBoxes)::value_type); }

		public: size_t GetTileCount() const { return boundingBoxes.size(); }

		
		public: Math::BoundingBox &GetTile(unsigned x, unsigned y, unsigned z);
		
	};

	
}
