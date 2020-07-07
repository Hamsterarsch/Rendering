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
		ComputeShaderAsset::GetAssetClassExtension()
	};
	
	const std::unordered_set<std::string> AssetTypesRegistry::typesNotCreatableByUser
	{
		ProjectAsset::GetAssetClassExtension(),
		CacheAsset::GetAssetClassExtension(),		
		ComputeShaderAsset::GetAssetClassExtension()
	};
	


	
	AssetTypesRegistry::AssetClassInfo::AssetClassInfo
	(
		Renderer::RendererFacade &iconTarget,
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
		displayIcon->UploadToRenderer(iconTarget);
		iconView.descriptorHandle = displayIcon->GetDescriptorHandle();
	}




	AssetTypesRegistry::AssetTypesRegistry
	(
		assetSystem::AssetSystem &addTargetAndIconSource,		
		Renderer::RendererFacade &iconUploadTarget
	)	:
		showAllTypes{ false }
	{
		RegisterAssetTypesWith(addTargetAndIconSource);
		
		AddAssetInfo<ImageAsset>(addTargetAndIconSource, iconUploadTarget, "Image Asset", "Images/Icons/TextureIcon.img", {});
				
		static const char *fileIconPath{ "Images/Icons/FileIcon.img" };
		AddAssetInfo<ProjectAsset>(addTargetAndIconSource, iconUploadTarget,		"Project Asset", fileIconPath, {});		
		AddAssetInfo<PixelShaderAsset>(addTargetAndIconSource, iconUploadTarget,	"Pixel Shader Asset", fileIconPath, MakeUnique<PrototypeEditor<Ui::User::ShaderEditorFrontend>>());
		AddAssetInfo<VertexShaderAsset>(addTargetAndIconSource, iconUploadTarget,	"Vertex Shader Asset", fileIconPath, MakeUnique<PrototypeEditor<Ui::User::ShaderEditorFrontend>>());
		AddAssetInfo<ComputeShaderAsset>(addTargetAndIconSource, iconUploadTarget,	"Compute Shader Asset", fileIconPath, MakeUnique<PrototypeEditor<Ui::User::ShaderEditorFrontend>>());
		
	}

		void AssetTypesRegistry::RegisterAssetTypesWith(assetSystem::AssetSystem &asys)
		{
			RegisterAsset<ProjectAsset>(asys);
			RegisterAsset<ImageAsset>(asys);
			RegisterAsset<PixelShaderAsset>(asys);
			RegisterAsset<VertexShaderAsset>(asys);
			RegisterAsset<ComputeShaderAsset>(asys);
							
		}

			template <class t_asset>
			void AssetTypesRegistry::RegisterAsset(assetSystem::AssetSystem &registerTarget)
			{
				registerTarget.RegisterAssetClass(t_asset::GetAssetClassExtension(), MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
			
			}

		template<class t_asset>
		void AssetTypesRegistry::AddAssetInfo
		(
			assetSystem::AssetSystem &iconSource,
			Renderer::RendererFacade &iconUploadTarget,
			const char *displayName,
			const char *iconImageAssetPath,
			AssetClassInfo::EditorProvider &&assetEditor,
			AssetClassInfo::ImportProvider &&importDialog
		)
		{
			if(assetClassMap.find(t_asset::GetAssetClassExtension()) != assetClassMap.end())
			{
				return;
			}
		
			assetClassInfos.emplace_back
			(
				iconUploadTarget,
				iconSource.GetAsset(iconImageAssetPath),
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



	UniquePtr<Ui::States::UiState> AssetTypesRegistry::MakeAssetEditor(const char *assetAbsolutePath, Ui::UiStateMachine &stateEditorParent, const assetSystem::AssetPtr &assetToEdit) const
	{
		std::filesystem::path path{ assetAbsolutePath };		
		auto assetClass{ path.replace_extension("").extension().string().erase(0, 1) };
				
		auto assetName{ path.filename().replace_extension().string() };

		const auto &provider{ assetClassInfos.at(assetClassMap.at(assetClass)).assetEditor };
		if(provider)
		{
			return provider->Clone(stateEditorParent, std::move(assetName), assetToEdit);
			
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
