#pragma once
#include "AssetSystem/Interface/Asset.hpp"
#include "Core/Version.hpp"


namespace App::Assets
{
	class ProjectAsset final : public assetSystem::Asset
	{
		private: Core::Version projectVersion;


		public: ProjectAsset();

		public: explicit ProjectAsset(const Core::Version &programVersion);
						
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;

		public: bool DoesProjectVersionMatch(const Core::Version &version) const { return projectVersion == version; }

		public: static const char *GetAssetClassExtension();

		public: static const wchar_t *GetAssetClassExtensionW();
		
		
	};
	
}