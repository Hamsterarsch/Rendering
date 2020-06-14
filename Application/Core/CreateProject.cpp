#include "Core/CreateProject.hpp"
#include "AssetTypes/ProjectAsset.hpp"
#include "AssetSystem/Interface/AssetConstructOperationsHelper.hpp"
#include "Core/Globals.hpp"
#include <string>
#include <filesystem>


namespace App::Core
{
	using namespace assetSystem;
	
	UniquePtr<AssetSystem> CreateProject(const char *projectName, const char *projectAssetFolder)
	{
		auto asys{ MakeAssetSystem(projectAssetFolder) };
		asys->RegisterAssetClass(Assets::ProjectAsset::GetAssetClassExtension(), MakeUnique<AssetConstructOperationsHelper<Assets::ProjectAsset>>());

		Assets::ProjectAsset asset{ globals.programVersion };
		std::string relativeAssetPath{ projectName };
		relativeAssetPath += '.';
		relativeAssetPath += Assets::ProjectAsset::GetAssetClassExtension();
		
		asys->MakeAsset(relativeAssetPath.c_str(), std::move(asset));

		return asys;
		
	}


	
	UniquePtr<AssetSystem> LoadProject(std::filesystem::path absoluteProjectAssetFilePath, bool &hasVersionMismatch)
	{		
		auto relativeAssetPath{ absoluteProjectAssetFilePath.filename().string() };
		absoluteProjectAssetFilePath.remove_filename();
				
		auto asys{ MakeAssetSystem(absoluteProjectAssetFilePath.string().c_str()) };
		asys->RegisterAssetClass(Assets::ProjectAsset::GetAssetClassExtension(), MakeUnique<AssetConstructOperationsHelper<Assets::ProjectAsset>>());
				
		const auto assetExtensionPos{ relativeAssetPath.find_last_of('.') };
		relativeAssetPath.erase(assetExtensionPos, relativeAssetPath.size() - assetExtensionPos);

		auto asset{ asys->GetAsset(relativeAssetPath.c_str()) };
		if(not reinterpret_cast<Assets::ProjectAsset *>(asset.GetAsset()) ->DoesProjectVersionMatch(globals.programVersion))
		{
			hasVersionMismatch = true;
			return {};	
			
		}

		hasVersionMismatch = false;				
		return asys;
			

		
	}

	
}
