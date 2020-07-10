#pragma once
#include "Asset.hpp"
#include "Resources/HandleWrapper.hpp"


namespace App::Assets
{
	class ShaderAsset;

	class PsoAsset final : public assetSystem::Asset
	{
		private: Renderer::HandleWrapper psoHandle;
		

		
		public: assetSystem::io::Archive& Serialize(assetSystem::io::Archive& archive) override { return archive; }
	};
	
	
}
