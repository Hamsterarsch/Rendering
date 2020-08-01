#pragma once
#include "Rendering/ShaderRelevantTypes/RenderingBoundingBox.hpp"
#include "Math/Interface/Types/Angle.hpp"
#include "Rendering/ShaderRelevantTypes/VolumeTileGridData.hpp"
#include <vector>


namespace App::Rendering
{
	class VolumeTileGrid
	{
		private: float fovTermForDepthCompute;
		
		private: Math::VectorUint3 gridsize;

		private: std::vector<RenderingBoundingBox> boundingBoxes;
			


		public: VolumeTileGrid() = default;
		
		public: VolumeTileGrid(const Math::VectorUint2 &approximateTileDimensions, Math::Angle verticalFov, float tileDepthScale, VolumeTileGridData &gridData);


		public: size_t SizeInBytes() const { return sizeof(decltype(boundingBoxes)::value_type) * boundingBoxes.size(); }
		
		public: void *GetData() { return boundingBoxes.data(); }

		public: size_t GetTileStride() const { return sizeof(decltype(boundingBoxes)::value_type); }

		public: size_t GetTileCount() const { return boundingBoxes.size(); }

		
		public: RenderingBoundingBox &GetTile(unsigned x, unsigned y, unsigned z);
		
	};

	
}
