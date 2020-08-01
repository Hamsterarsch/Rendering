#pragma once
#include "RendererAsset.hpp"
#include "Resources/HandleWrapper.hpp"
#include "UserPixelShaderAsset.hpp"


namespace App::Assets
{
	class ShaderAsset;

	enum class PipelineType : unsigned char
	{
		Opaque,
		Translucent
	};
	
	class PipelineAsset final : public RendererAsset
	{
		private: PipelineType type;

		private: Renderer::HandleWrapper signatureHandle;
		
		private: Renderer::HandleWrapper psoHandle;

		private: assetSystem::AssetPtrTyped<VertexShaderAsset> vertexShader;
		
		private: assetSystem::AssetPtrTyped<UserPixelShaderAsset> pixelShader;


		
		public: PipelineAsset() : type{ PipelineType::Opaque } {}

		public: PipelineAsset(const assetSystem::AssetPtrTyped<VertexShaderAsset> &vertexShader, const assetSystem::AssetPtrTyped<UserPixelShaderAsset> &pixelShader)
			:
			type{ PipelineType::Opaque },
			pixelShader{ pixelShader }
		{}

				
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;

		public: void UploadToRenderer(Renderer::RendererFacade &renderer) override;
							   		
			private: void MakeOpaquePipelineSettings(Renderer::RendererFacade &renderer) const;

		public: Renderer::ResourceHandle::t_hash GetPsoHandle() const { return psoHandle; }

		public: Renderer::ResourceHandle::t_hash GetSignatureHandle() const { return signatureHandle; }
		
		public: bool OccludesAllPixelsRenderedBehind() const;
					
		
		public: static const char *GetAssetClassExtension();

	};
	
	
}
