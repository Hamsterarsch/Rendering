#include "Rendering/GraphVisitorHarvestMeshes.hpp"
#include "Scene/ContentMesh.hpp"
#include "AssetTypes/StaticMeshAsset.hpp"
#include "Scene/ContentLight.hpp"


namespace App::Rendering
{
	void GraphVisitorHarvesMeshes::Visit(Scene::ContentMesh &mesh)
	{
		const auto asset{ mesh.GetMeshAsset() };

		auto meshDataSlot{ meshInstanceData.find(asset) };
		if(meshDataSlot == meshInstanceData.end())
		{			
			meshInstanceData.insert( {asset, MeshShardData{{std::vector<ShardRenderData>(asset->GetNumMeshShards())}} } );
			meshDataSlot = meshInstanceData.find(asset);
		}
		
		for(unsigned meshShardIndex{ 0 }; meshShardIndex < asset->GetNumMeshShards(); ++meshShardIndex)
		{			
			meshDataSlot->second.meshShardData.at(meshShardIndex).pipelineOccurences[mesh.GetPipelineForMeshShardAt(meshShardIndex)].data.emplace_back(ShardInstanceData{ GetCurrentNodeToWorld(), mesh.GetInstanceData() });			
		}
		
	}


	
	void GraphVisitorHarvesMeshes::Visit(Scene::ContentLight &light)
	{
		lightData.emplace_back(light.GetLightData());
		
	}

	
}
