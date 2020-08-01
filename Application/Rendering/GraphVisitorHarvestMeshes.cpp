#include "Rendering/GraphVisitorHarvestMeshes.hpp"
#include "Scene/ContentMesh.hpp"
#include "AssetTypes/StaticMeshAsset.hpp"
#include "Scene/ContentLight.hpp"


namespace App::Rendering
{
	void GraphVisitorHarvestMeshes::Visit(Scene::ContentMesh &mesh)
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
			meshDataSlot->second.transforms.emplace_back(GetCurrentNodeToWorld());
			meshDataSlot->second.meshShardData.at(meshShardIndex).pipelineOccurences[mesh.GetPipelineForMeshShardAt(meshShardIndex)].data
				.emplace_back(MeshShardInstanceData{ static_cast<uint32_t>(meshDataSlot->second.transforms.size()-1), mesh.GetInstanceData() });			
		}
		
	}


	
	void GraphVisitorHarvestMeshes::Visit(Scene::ContentLight &light)
	{		
		lightData.emplace_back(light.GetLightData(GetCurrentNodeToWorld()));
		
	}

	
}
