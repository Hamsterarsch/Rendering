#pragma once
#include "Shared/PtrTypes.hpp"
#include "AssetSystem/Interface/AssetSystem.hpp"
#include <filesystem>


namespace assetSystem
{
	class AssetSystem;
}


namespace App::Core
{
	class Version;

	
	UniquePtr<assetSystem::AssetSystem> CreateProject(const char *projectName, const char *projectAssetFolder, const Version &programVersion);

	UniquePtr<assetSystem::AssetSystem> LoadProject(std::filesystem::path absoluteProjectAssetFilePath, const Version &programVersion);

	
}
