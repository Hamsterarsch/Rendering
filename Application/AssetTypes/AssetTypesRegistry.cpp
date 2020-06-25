#include "AssetTypes/AssetTypesRegistry.hpp"
#include <filesystem>
#include "ShaderAsset.hpp"
#include "AssetSystem.hpp"
#include "AssetConstructOperationsHelper.hpp"


namespace App::Assets
{	
	AssetTypesRegistry::AssetTypesRegistry(Core::Application &app)
	{
		AddAssetInfo<ImageAsset>(app, "img", "Image Asset", "Images/Icons/TextureIcon.img");
		AddAssetInfo<ShaderAsset>(app, "shdr", "Shader Asset", "Images/Icons/FileIcon.img");
				
	}

		template<class t_asset>
		void AssetTypesRegistry::AddAssetInfo(Core::Application &app, const char *classExtension, const char *displayName, const char *iconImageAssetPath)
		{
			app.GetProgramAssets().RegisterAssetClass(classExtension, MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
			app.GetProjectAssets().RegisterAssetClass(classExtension, MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
		
			assetClassInfos.insert( {classExtension, { app.GetProgramAssets().GetAsset(iconImageAssetPath), displayName }} );
		
		}


	
	const char *AssetTypesRegistry::GetAssetClassDisplayName(const char *classExtension) const
	{
		return assetClassInfos.at(classExtension).displayName;
		
	}


	
	UniquePtr<Ui::States::UiState> AssetTypesRegistry::GetAssetEditor(const char* assetAbsolutePath) const
	{
		std::filesystem::path path{ assetAbsolutePath };
		auto assetClass{ path.replace_extension("").extension().string() };

		if(assetClass == "shdr")
		{
			return {};
			
		}
		else
		{
			return {};
			
		}	
		
	}


	
	bool AssetTypesRegistry::IsHiddenAssetType(const char *classExtension) const
	{
		return hiddenAssetTypes.find(classExtension) != hiddenAssetTypes.end();
		
	}

	
}
