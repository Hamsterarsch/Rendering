#pragma once
#include "AssetFactories/MeshData.hpp"
#include "Types/Vector.hpp"
#include "Archivable.hpp"
#include <vector>


namespace App::Assets
{	
	class StaticMeshData final : public MeshData, public assetSystem::Archivable
	{
		public: struct VertexData
		{
			Math::Vector3 pos;
			Math::Vector3 normal;
			Math::Vector2 uv;			
		};
		static_assert
		(
			sizeof(VertexData)== sizeof VertexData::pos + sizeof VertexData::normal + sizeof VertexData::uv 
			&& std::is_standard_layout_v<VertexData>, 
			"Wrong vertex data alignment"
		);		
		
		public: std::vector<VertexData> vertices;
		
		public: std::vector<unsigned> indices;


				 				
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
		
		public: const void *GetVertexData() const override { return vertices.data(); }

		public: size_t GetVertexDataSizeInBytes() const override { return vertices.size() * sizeof(decltype(vertices)::value_type); }

		public: const unsigned *GetIndexData() const override { return indices.data(); }

		public: unsigned GetNumIndices() const override { return indices.size(); }

		public: void ConfigureVertexLayout(Renderer::VertexLayoutSettings &forSettings) override;

		
	};
	
	
}
