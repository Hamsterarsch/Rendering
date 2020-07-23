#pragma once
#include "Asset.hpp"
#include "Resources/HandleWrapper.hpp"
#include "UserPixelShaderAsset.hpp"


namespace App::Assets
{
	class ShaderAsset;

	enum class PsoType : unsigned char
	{
		Opaque,
		Translucent
	};
	
	class PsoAsset final : public assetSystem::Asset
	{
		private: PsoType type;
		
		private: Renderer::HandleWrapper psoHandle;

		private: assetSystem::AssetPtrTyped<UserPixelShaderAsset> pixelShader;



		public: PsoAsset() : type{ PsoType::Opaque } {}

		public: PsoAsset(const assetSystem::AssetPtrTyped<UserPixelShaderAsset> &pixelShader) : type{ PsoType::Opaque }, pixelShader{ pixelShader } {}

				
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;

		public: bool OccludesAllPixelsRenderedBehind() const;
		
		public: static const char *GetAssetClassExtension();
		
	};
	
	
}
