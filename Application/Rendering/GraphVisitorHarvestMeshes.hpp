#pragma once
#include "Scene/GraphVisitorTransformRequiring.h"
#include <unordered_map>
#include "AssetPtr.hpp"
#include "ShaderRelevantTypes/Light.hpp"
#include "Shared/PtrTypes.hpp"
#include "Scene/MeshInstanceData.hpp"
#include "AssetTypes/StaticMeshAsset.hpp"
#include "AssetTypes/PsoAsset.hpp"


namespace App::Rendering
{
	struct AssetPtrHasher
	{
		public: size_t operator()(const assetSystem::AssetPtr &ptr) const
		{
			return ptr.GetCurrentKey();
		}
	};
	
	class GraphVisitorHarvestMeshes final : public Scene::GraphVisitorTransformRequiring
	{
		private: struct ShardInstanceData
		{
			Math::Matrix transform;
			Scene::MeshInstanceData perInstanceData;

			
		};
		
		private: struct ShardRenderData
		{
			private: struct Bucket
			{
				std::vector<ShardInstanceData> data;
			};
			
			public: std::unordered_map<assetSystem::AssetPtrTyped<Assets::PsoAsset>, Bucket, AssetPtrHasher> pipelineOccurences;

			ShardRenderData() = default;

			ShardRenderData(const ShardRenderData &) = delete;

			ShardRenderData(ShardRenderData &&) = default;

			ShardRenderData &operator=(const ShardRenderData &) = delete;

			ShardRenderData &operator=(ShardRenderData &&) = default;

			~ShardRenderData() = default;
			
		};
		
		private: struct MeshShardData
		{
			std::vector<ShardRenderData> meshShardData;

			MeshShardData() = default;

			MeshShardData(const MeshShardData &) = delete;

			MeshShardData(MeshShardData &&) = default;

			MeshShardData &operator=(const MeshShardData &) = delete;

			MeshShardData &operator=(MeshShardData &&) = default;

			~MeshShardData() = default;
						
		};
		
		public: std::unordered_map<assetSystem::AssetPtrTyped<Assets::StaticMeshAsset>, MeshShardData, AssetPtrHasher> meshInstanceData;//todo make interface for access
				
		private: std::vector<Light> lightData;


		public: GraphVisitorHarvestMeshes() = default;
		
		public: GraphVisitorHarvestMeshes(const GraphVisitorHarvestMeshes&) = delete;

		public: GraphVisitorHarvestMeshes(GraphVisitorHarvestMeshes&&) = default;
		
		
		public: void Visit(Scene::ContentMesh &mesh) override;
		
		public: void Visit(Scene::ContentLight &light) override;
		
	};

	
}
