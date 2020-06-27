#pragma once
#include <unordered_map>
#include "ImageAsset.hpp"
#include "AssetPtr.hpp"
#include <unordered_set>
#include "Ui/States/UiState.hpp"
#include "Core/Application.hpp"
#include "Core/ImageView.hpp"
#include "Core/Prototype.hpp"


namespace App::Assets
{
	class AssetTypesRegistry
	{
		private: struct AssetClassInfo
		{
			using EditorProvider = UniquePtr<Core::Prototype<Ui::States::UiState>>;
			
			assetSystem::AssetPtrTyped<ImageAsset> displayIcon;
			const char *displayName;
			const char *extension;
			EditorProvider assetEditor;
			EditorProvider assetImportDialog;			
			Core::ImageView iconView;

			AssetClassInfo
			(
				Core::Application &app,
				const assetSystem::AssetPtrTyped<ImageAsset> &icon,
				const char *displayName,
				const char *extension,
				EditorProvider &&assetEditor,
				EditorProvider &&importDialog
			);
										
		};
		
		std::unordered_map<std::string, size_t> assetClassMap;

		std::vector<AssetClassInfo> assetClassInfos;
		
		inline static const std::unordered_set<const char *> hiddenAssetTypes{ "cache" };

		
		
		public: AssetTypesRegistry() = default;
		
		public: AssetTypesRegistry(Core::Application &app);

			private: template<class t_asset> void AddAssetInfo
			(
				Core::Application &app,
				const char *displayName,
				const char *iconImageAssetPath,
				AssetClassInfo::EditorProvider &&assetEditor,
				AssetClassInfo::EditorProvider &&importDialog = {}
			);

		
		public: size_t GetNumberAssetTypes() const { return assetClassInfos.size(); }

		public: const Core::ImageView &GetAssetIcon(size_t index) const { return assetClassInfos.at(index).iconView; }
		
		public: bool IsHiddenAssetType(size_t index) const;

		public: const char *GetAssetTypeDisplayName(size_t index) const;

		public: UniquePtr<Ui::States::UiState> GetAssetImportDialog(size_t index) const;
		

		public: UniquePtr<Ui::States::UiState> GetAssetEditor(const char *assetAbsolutePath) const;

		public: Core::ImageView GetAssetIcon(const char *classExtension) const;

		public: bool IsHiddenAssetType(const char *classExtension) const;
				
	};

	
}
