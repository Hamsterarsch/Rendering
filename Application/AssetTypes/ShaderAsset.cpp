#include "AssetTypes/ShaderAsset.hpp"
#include "AssetSystem/Interface/IO/Archive.hpp"


namespace App::Assets
{
	ShaderAsset::ShaderAsset() {}
	

	assetSystem::io::Archive &ShaderAsset::Serialize(assetSystem::io::Archive &archive)
	{
		if(archive.IsReading())
		{
			shaderCode.resize(archive.GetPropertySizeInBytes("code"));
		}
				
		return archive.Serialize("code", shaderCode.data());
		
	}


	
	const char *ShaderAsset::GetAssetClassExtension()
	{
		static const char *extension{ "shdr" };

		return extension;
		
	}


	
	const wchar_t *ShaderAsset::GetAssetClassExtensionW()
	{
		static const wchar_t *extension{ L"shdr" };

		return extension;
		
	}

	
}
