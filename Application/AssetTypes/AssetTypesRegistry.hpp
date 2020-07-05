#pragma once
#include <unordered_map>
#include "ImageAsset.hpp"
#include "AssetPtr.hpp"
#include <unordered_set>
#include "Core/Application.hpp"
#include "Core/ImageView.hpp"
#include "Core/Prototype.hpp"
#include "Ui/States/UiEditorState.hpp"
#include "Ui/States/UiSimpleState.hpp"


namespace App::Assets
{
	class PrototypeEditorBase
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(PrototypeEditorBase)

		public: virtual UniquePtr<Ui::States::UiState> Clone(Ui::UiStateMachine &parent, std::string &&assetName, const assetSystem::AssetPtr &assetToEdit) const = 0;
		
	};
	
	template<class t_editor>
	class PrototypeEditor final : public PrototypeEditorBase
	{
		static_assert(std::is_base_of_v<Ui::Core::UiFrontend, t_editor>, "editor type is no subclass of UiFrontend");
		
		public: UniquePtr<Ui::States::UiState> Clone(Ui::UiStateMachine &parent, std::string &&assetName, const assetSystem::AssetPtr &assetToEdit) const override
		{
			auto state{ MakeUnique<Ui::States::UiSimpleState>(parent) };
			state->SetFrontend(MakeUnique<t_editor>(*state, std::move(assetName), assetToEdit));			
			
			return state;
			
		}
		
	};




	
	class AssetTypesRegistry
	{
		private: struct AssetClassInfo
		{
			using EditorProvider = UniquePtr<PrototypeEditorBase>;
			using DefaultAssetProvider = UniquePtr<Core::Prototype<assetSystem::Asset>>;
			using ImportProvider = UniquePtr<Core::Prototype<Ui::States::UiState>>;
			
			assetSystem::AssetPtrTyped<ImageAsset> displayIcon;
			const char *displayName;
			const char *extension;
			DefaultAssetProvider defaultAsset;
			EditorProvider assetEditor;
			ImportProvider assetImportDialog;
			Core::ImageView iconView;

			AssetClassInfo
			(
				Core::Application &app,
				const assetSystem::AssetPtrTyped<ImageAsset> &icon,
				const char *displayName,
				const char *extension,
				DefaultAssetProvider &&defaultAsset,
				EditorProvider &&assetEditor,
				ImportProvider &&importDialog
			);
										
		};

		Core::Application *app;
		
		std::unordered_map<std::string, size_t> assetClassMap;

		std::vector<AssetClassInfo> assetClassInfos;
		
		static const std::unordered_set<std::string> hiddenAssetTypes;

		static const std::unordered_set<std::string> typesNotCreatableByEditor;

		
		
		public: AssetTypesRegistry() : app{ nullptr } {}
		
		public: AssetTypesRegistry(Core::Application &app);

			private: template<class t_asset> void AddAssetInfo
			(
				Core::Application &app,
				const char *displayName,
				const char *iconImageAssetPath,
				AssetClassInfo::EditorProvider &&assetEditor,
				AssetClassInfo::ImportProvider &&importDialog = {}
			);

		
		public: size_t GetNumberAssetTypes() const { return assetClassInfos.size(); }

		public: const Core::ImageView &GetAssetIcon(size_t index) const { return assetClassInfos.at(index).iconView; }
		
		public: bool IsHiddenAssetType(size_t index) const;

		public: bool IsUserCreatableType(size_t index) const;

		public: const char *GetAssetTypeDisplayName(size_t index) const;

		public: UniquePtr<Ui::States::UiState> GetAssetImportDialog(size_t index) const;

		public: UniquePtr<assetSystem::Asset> GetDefaultAssetOfType(size_t index) const;

		public: const char *GetAssetClassExtension(size_t index) const;
		

		public: UniquePtr<Ui::States::UiState> MakeAssetEditor(const char *assetAbsolutePath, const assetSystem::AssetPtr &assetToEdit) const;

		public: Core::ImageView GetAssetIcon(const char *classExtension) const;

		public: bool IsHiddenAssetType(const char *classExtension) const;
				
	};

	
}
