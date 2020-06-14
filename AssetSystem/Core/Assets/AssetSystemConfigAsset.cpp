#include "AssetSystem/Core/Assets/AssetSystemConfigAsset.hpp"
#include "AssetSystem/IO/Filetypes/AssetReader.hpp"


namespace assetSystem
{
	Assets::AssetSystemConfigAsset::AssetSystemConfigAsset(const std::filesystem::path &configDirectory)
	{		
		io::AssetReader assetReader{ configDirectory.string() + "AssetSystemConfig.asset" };

		const auto length{ assetReader.GetPropertySizeInBytes(assetDirectoryPropertyName) };
		assetDirectory.resize(length);

		assetReader.Serialize(assetDirectoryPropertyName, assetDirectory.data());
				
	}

	std::filesystem::path Assets::AssetSystemConfigAsset::GetAssetDirectory() const
	{
		return { assetDirectory };
		
	}

	
}
