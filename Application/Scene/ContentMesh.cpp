#include "Scene/ContentMesh.hpp"
#include "Scene/GraphVisitor.hpp"


namespace App::Scene
{
	ContentMesh::ContentMesh
	(
		const assetSystem::AssetPtrTyped<Assets::StaticMeshAsset> &mesh,
		const assetSystem::AssetPtrTyped<Assets::PsoAsset> &defaultPso
	)
		:
		mesh{ mesh }
	{
		for(unsigned shardIndex{ 0 }; shardIndex < mesh->GetNumMeshShards(); ++shardIndex)
		{
			meshShardPipelines.emplace_back(defaultPso);
		}
		
	}


	
	void ContentMesh::Accept(GraphVisitor &visitor)
	{
		visitor.Visit(*this);		
		
	}

	
}
