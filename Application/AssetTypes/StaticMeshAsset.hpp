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

		//todo: find a better name for submeshes/matSlots
		public: unsigned GetNumMeshShards() const { return static_cast<unsigned>(meshData.submeshes.size()); }

		public: MeshSubgroup GetMaterialSlotIndexInfoFor(unsigned slotIndex) const { return meshData.submeshes.at(slotIndex); }

		
		public: static const char *GetAssetClassExtension();

	};

	
}
