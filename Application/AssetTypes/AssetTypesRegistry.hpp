#pragma once
#include <unordered_map>
#include "ImageAsset.hpp"
#include "AssetPtr.hpp"
#include <unordered_set>
#include "Ui/States/UiState.hpp"
#include "Core/Application.hpp"


namespace App::Assets
{
	class AssetTypesRegistry
	{
		private: struct AssetClassInfo
		{
			assetSystem::AssetPtrTyped<ImageAsset> displayIcon;
			const char *displayName;
						
		};
		
		std::unordered_map<const char *, AssetClassInfo> assetClassInfos;

		inline static const std::unordered_set<const char *> hiddenAssetTypes{ "cache" };

		
		
		public: AssetTypesRegistry() = default;
		
		public: AssetTypesRegistry(App::Core::Application &app);

		private: template<class t_asset> void AddAssetInfo(Core::Application &app, const char *classExtension, const char *displayName, const char *iconImageAssetPath);

		public: const char *GetAssetClassDisplayName(const char *classExtension) const;

		public: UniquePtr<Ui::States::UiState> GetAssetEditor(const char *assetAbsolutePath) const;

		public: bool IsHiddenAssetType(const char *classExtension) const;
				
	};
	   
	
}
