#pragma once
#include "Renderer/Interface/Resources/HandleWrapper.hpp"
#include "Asset.hpp"
#include "RendererAsset.hpp"
#include "AssetFactories/StaticMeshData.hpp"


namespace Renderer
{
	class RendererFacade;
	namespace Commands { class CompositeCommand; }
}


namespace App::Assets
{	
	class StaticMeshAsset final : public assetSystem::Asset, public RendererAsset
	{
		private: Renderer::HandleWrapper bufferHandle;
				 		
		private: StaticMeshData meshData;


		
		public: StaticMeshAsset() = default;

		public: StaticMeshAsset(StaticMeshData &&data) : meshData{ std::move(data) } {}


		public: void OnMakeAsset(const char *absoluteAssetFilePath) override {}
		
		public: void OnAssetLoaded(const char *absoluteAssetFilePath) override {}
				
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;

		public: void UploadToRenderer(Renderer::RendererFacade &renderer) override;

		
		public: Renderer::ResourceHandle::t_hash GetMeshBuffer() const { return bufferHandle; }

		public: size_t GetMeshBufferVertexCount() const { return meshData.GetVertexCount(); }

		public: unsigned GetVertexStrideInBytes() const { return meshData.GetVertexStrideInBytes(); }


		public: size_t GetByteOffsetToIndexData() const { return meshData.GetVertexDataSizeInBytes(); }

		public: unsigned GetNumIndices() const { return meshData.GetNumIndices(); }

		public: unsigned GetIndexSizeInBytes() const { return sizeof(decltype(meshData.indices)::value_type); }

		public: Renderer::Format GetIndexFormat() const { return &Renderer::FormatTargets::R32_Uint; static_assert(sizeof(decltype(meshData.indices)::value_type) == 4); }
		
		
		public: unsigned GetNumMeshShards() const { return static_cast<unsigned>(meshData.submeshes.size()); }

		public: MeshSubgroup GetMeshShardInfo(unsigned shardIndex) const { return meshData.submeshes.at(shardIndex); }

		
		public: static const char *GetAssetClassExtension();

	};

	
}
