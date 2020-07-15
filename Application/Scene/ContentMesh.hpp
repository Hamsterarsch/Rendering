#pragma once
#include "GraphNodeContent.hpp"
#include "AssetTypes/StaticMeshAsset.hpp"
#include "AssetPtr.hpp"
#include "AssetTypes/PsoAsset.hpp"
#include "Scene/MeshInstanceData.hpp"


namespace App::Scene
{	
	class ContentMesh final : public GraphNodeContent
	{		
		private: assetSystem::AssetPtrTyped<Assets::StaticMeshAsset> mesh;

		private: std::vector<assetSystem::AssetPtrTyped<Assets::PsoAsset>> meshShardPipelines;
				 		
		

		public: explicit ContentMesh
		(
			const assetSystem::AssetPtrTyped<Assets::StaticMeshAsset> &mesh,
			const assetSystem::AssetPtrTyped<Assets::PsoAsset> &defaultPso
		);

		
		public: void Accept(GraphVisitor &visitor) override;

		public: assetSystem::AssetPtrTyped<Assets::StaticMeshAsset> GetMeshAsset() const { return mesh; }

		public: assetSystem::AssetPtrTyped<Assets::PsoAsset> GetPipelineForMeshShardAt(unsigned index) { return meshShardPipelines.at(index); }

		public: MeshInstanceData GetInstanceData() const { return {}; }
		
	};
	
	
}
