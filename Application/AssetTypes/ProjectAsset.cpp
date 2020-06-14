#include "AssetTypes/ProjectAsset.hpp"
#include "AssetSystem/Interface/IO/Archive.hpp"


namespace App::Assets
{
	ProjectAsset::ProjectAsset() : projectVersion{ 0, 0, 0 }
	{
	}


	
	ProjectAsset::ProjectAsset(const Core::Version &programVersion) : projectVersion{ programVersion }
	{
	}


	
	assetSystem::io::Archive &ProjectAsset::Serialize(assetSystem::io::Archive &archive)
	{
		return archive.Serialize("version", projectVersion);
		
	}


	
	const char *ProjectAsset::GetAssetClassExtension()
	{
		static const char *extension{ "proj" };

		return extension;
		
	}


	
	const wchar_t *ProjectAsset::GetAssetClassExtensionW()
	{
		static const wchar_t *extension{ L"proj" };

		return extension;
		
	}

	
}
