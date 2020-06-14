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
	UniquePtr<assetSystem::AssetSystem> CreateProject(const char *projectName, const char *projectAssetFolder);

	UniquePtr<assetSystem::AssetSystem> LoadProject(std::filesystem::path absoluteProjectAssetFilePath, bool &hasVersionMismatch);

	
}
