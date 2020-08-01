#pragma once
#include "AssetSystemTypes.hpp"
#include <string>


namespace assetSystem::core
{	
	AssetKey MakeAssetKeyWithPathCleaning(const char *assetPathRelativeToRootFolder);
	
	std::string EnsureCorrectPathFormat(const char *path);

	
	AssetKey MakeAssetKey(const std::string &cleanPath);

	
}
