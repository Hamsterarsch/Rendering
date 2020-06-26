#include "AssetTypes/AssetTypesRegistry.hpp"
#include <filesystem>
#include "ShaderAsset.hpp"
#include "AssetSystem.hpp"
#include "AssetConstructOperationsHelper.hpp"
#include "ProjectAsset.hpp"


namespace App::Assets
{
	AssetTypesRegistry::AssetClassInfo::AssetClassInfo
	(
		Core::Application &app,
		const assetSystem::AssetPtrTyped<ImageAsset> &icon,
		const char *displayName,
		const char *extension
	)
		:
		displayIcon{ icon },
		displayName{ displayName },
		extension{ extension }			
	{
		displayIcon->UploadToRenderer(app.GetRenderer());
		iconView.descriptorHandle = displayIcon->GetDescriptorHandle();
	}
	

	AssetTypesRegistry::AssetTypesRegistry(Core::Application &app)
	{
		AddAssetInfo<ImageAsset>(app, "Image Asset", "Images/Icons/TextureIcon.img");
		AddAssetInfo<ProjectAsset>(app, "Project Asset", "Images/Icons/FileIcon.img");
		AddAssetInfo<ShaderAsset>(app, "Shader Asset", "Images/Icons/FileIcon.img");
				
	}

		template<class t_asset>
		void AssetTypesRegistry::AddAssetInfo(Core::Application &app, const char *displayName, const char *iconImageAssetPath)
		{
			app.GetProgramAssets().RegisterAssetClass(t_asset::GetAssetClassExtension(), MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
			app.GetProjectAssets().RegisterAssetClass(t_asset::GetAssetClassExtension(), MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
		
			assetClassInfos.emplace_back(app, app.GetProgramAssets().GetAsset(iconImageAssetPath), displayName, t_asset::GetAssetClassExtension());					
			assetClassMap.insert( {t_asset::GetAssetClassExtension(), assetClassInfos.size()-1} );
		
		}



	bool AssetTypesRegistry::IsHiddenAssetType(size_t index) const
	{
		return hiddenAssetTypes.find(assetClassInfos.at(index).extension) != hiddenAssetTypes.end();
		
	}


	
	const char *AssetTypesRegistry::GetAssetTypeDisplayName(size_t index) const
	{
		return assetClassInfos.at(index).displayName;
		
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



	Core::ImageView AssetTypesRegistry::GetAssetIcon(const char *classExtension) const
	{		
		return assetClassInfos.at(assetClassMap.at(classExtension)).iconView;
		
	}

	
	
	bool AssetTypesRegistry::IsHiddenAssetType(const char *classExtension) const
	{
		return hiddenAssetTypes.find(classExtension) != hiddenAssetTypes.end();
		
	}

	
}
