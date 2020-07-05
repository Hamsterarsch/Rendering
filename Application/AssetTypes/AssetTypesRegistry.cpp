#include "AssetTypes/AssetTypesRegistry.hpp"
#include <filesystem>
#include "ShaderAsset.hpp"
#include "AssetSystem.hpp"
#include "AssetConstructOperationsHelper.hpp"
#include "ProjectAsset.hpp"
#include "AssetFileending.hpp"
#include "Ui/User/ShaderEditor.hpp"
#include "CacheAsset.hpp"




namespace App::Assets
{
	template<class t_asset>
	class PrototypeAsset final : public Core::Prototype<assetSystem::Asset>
	{
		public: UniquePtr<assetSystem::Asset> Clone() const override
		{
			return MakeUnique<t_asset>();
			
		}
		
	};

	
	

	const std::unordered_set<std::string> AssetTypesRegistry::hiddenAssetTypes
	{
		CacheAsset::GetAssetClassExtension(),
		ShaderAsset::GetAssetClassExtension(),
		ComputeShaderAsset::GetAssetClassExtension()
	};
	
	const std::unordered_set<std::string> AssetTypesRegistry::typesNotCreatableByUser
	{
		ProjectAsset::GetAssetClassExtension(),
		CacheAsset::GetAssetClassExtension(),
		ShaderAsset::GetAssetClassExtension(),
		ComputeShaderAsset::GetAssetClassExtension()
	};
	
	
	AssetTypesRegistry::AssetClassInfo::AssetClassInfo
	(
		Core::Application &app,
		const assetSystem::AssetPtrTyped<ImageAsset> &icon,
		const char *displayName,
		const char *extension,
		DefaultAssetProvider &&defaultAsset,
		EditorProvider &&assetEditor,
		ImportProvider &&importDialog
	)
		:
		displayIcon{ icon },
		displayName{ displayName },
		extension{ extension },
		defaultAsset{ std::move(defaultAsset) },
		assetEditor{ std::move(assetEditor) },
		assetImportDialog{ std::move(importDialog) }
	{
		displayIcon->UploadToRenderer(app.GetRenderer());
		iconView.descriptorHandle = displayIcon->GetDescriptorHandle();
	}
	

	AssetTypesRegistry::AssetTypesRegistry(Core::Application &app, bool showAllTypes) : app{ &app }, showAllTypes{ showAllTypes }
	{
		AddAssetInfo<ImageAsset>(app, "Image Asset", "Images/Icons/TextureIcon.img", {});
		AddAssetInfo<ProjectAsset>(app, "Project Asset", "Images/Icons/FileIcon.img", {});
		AddAssetInfo<ShaderAsset>(app, "Shader Asset", "Images/Icons/FileIcon.img", MakeUnique<PrototypeEditor<Ui::User::ShaderEditorFrontend>>());
		AddAssetInfo<PixelShaderAsset>(app, "Pixel Shader Asset", "Images/Icons/FileIcon.img", MakeUnique<PrototypeEditor<Ui::User::ShaderEditorFrontend>>());
		AddAssetInfo<VertexShaderAsset>(app, "Pixel Shader Asset", "Images/Icons/FileIcon.img", MakeUnique<PrototypeEditor<Ui::User::ShaderEditorFrontend>>());
		AddAssetInfo<ComputeShaderAsset>(app, "Compute Shader Asset", "Images/Icons/FileIcon.img", MakeUnique<PrototypeEditor<Ui::User::ShaderEditorFrontend>>());
				
	}

		template<class t_asset>
		void AssetTypesRegistry::AddAssetInfo
		(
			Core::Application &app,
			const char *displayName,
			const char *iconImageAssetPath,
			AssetClassInfo::EditorProvider &&assetEditor,
			AssetClassInfo::ImportProvider &&importDialog
		)
		{
			app.GetProgramAssets().RegisterAssetClass(t_asset::GetAssetClassExtension(), MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
			app.GetProjectAssets().RegisterAssetClass(t_asset::GetAssetClassExtension(), MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
		
			assetClassInfos.emplace_back
			(
				app,
				app.GetProgramAssets().GetAsset(iconImageAssetPath),
				displayName,
				t_asset::GetAssetClassExtension(),
				MakeUnique<PrototypeAsset<t_asset>>(),
				std::move(assetEditor),
				std::move(importDialog)
			);					
			assetClassMap.insert( {t_asset::GetAssetClassExtension(), assetClassInfos.size()-1} );
		
		}



	bool AssetTypesRegistry::IsHiddenAssetType(const size_t index) const
	{
		return not showAllTypes && hiddenAssetTypes.find(assetClassInfos.at(index).extension) != hiddenAssetTypes.end();
		
	}

	
	bool AssetTypesRegistry::IsUserCreatableType(size_t index) const
	{
		return showAllTypes || typesNotCreatableByUser.find(assetClassInfos.at(index).extension) == typesNotCreatableByUser.end();
				
	}



	const char *AssetTypesRegistry::GetAssetTypeDisplayName(const size_t index) const
	{
		return assetClassInfos.at(index).displayName;
		
	}


	
	UniquePtr<Ui::States::UiState> AssetTypesRegistry::GetAssetImportDialog(const size_t index) const
	{
		if(assetClassInfos.at(index).assetImportDialog)
		{
			return assetClassInfos.at(index).assetImportDialog->Clone();
			
		}
		return {};
		
	}


	
	UniquePtr<assetSystem::Asset> AssetTypesRegistry::GetDefaultAssetOfType(const size_t index) const
	{
		if(assetClassInfos.at(index).defaultAsset)
		{
			return assetClassInfos.at(index).defaultAsset->Clone();
		}
		return {};
		
	}


	
	const char *AssetTypesRegistry::GetAssetClassExtension(const size_t index) const
	{
		return assetClassInfos.at(index).extension;
		
	}



	UniquePtr<Ui::States::UiState> AssetTypesRegistry::MakeAssetEditor(const char *assetAbsolutePath, const assetSystem::AssetPtr &assetToEdit) const
	{
		std::filesystem::path path{ assetAbsolutePath };		
		auto assetClass{ path.replace_extension("").extension().string().erase(0, 1) };
				
		auto assetName{ path.filename().replace_extension().string() };

		const auto &provider{ assetClassInfos.at(assetClassMap.at(assetClass)).assetEditor };
		if(provider)
		{
			return provider->Clone(app->GetUiStateMachine(), std::move(assetName), assetToEdit);
			
		}
		return {};
		
	}



	Core::ImageView AssetTypesRegistry::GetAssetIcon(const char *classExtension) const
	{		
		return assetClassInfos.at(assetClassMap.at(classExtension)).iconView;
		
	}

	
	
	bool AssetTypesRegistry::IsHiddenAssetType(const char *classExtension) const
	{
		return not showAllTypes && hiddenAssetTypes.find(classExtension) != hiddenAssetTypes.end();
		
	}

	
}
