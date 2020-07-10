#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include <filesystem>
#include "AssetPtr.hpp"
#include "AssetTypes/ImageAsset.hpp"
#include "Core/ImageView.hpp"
#include "Core/Application.hpp"


namespace App::Ui::Core { class UiLayoutElement; }


namespace App::Ui::User
{
	class AssetBrowserFrontend final : public Core::UiFrontend
	{
		private: App::Core::Application *app;
		
		private: std::filesystem::path currentPath;

		private: std::filesystem::path rootPath;

		private: struct IconInfo
		{
			assetSystem::AssetPtrTyped<Assets::ImageAsset> image;			
			App::Core::ImageView view;

			IconInfo(Renderer::RendererFacade &uploadTarget, assetSystem::AssetPtrTyped<Assets::ImageAsset> &&image);
		};
		
		private: IconInfo iconFolder;

		private: Core::UiLayoutElement *content;

		private: struct Item
		{
			bool wasClicked;
			std::filesystem::path path;			

			Item(const std::filesystem::path &path);
			
		};
		
		private: std::vector<Item> itemList;

		private: bool shouldGoUp;

		private: bool shouldMakeNewAsset;

		private: bool wasNewAssetCreated;

				 		

		
		public: AssetBrowserFrontend(const char *initialAbsolutePath, App::Core::Application &app);

			private: void DisplayCurrentPathContents();

				private: void AddDisplay(const std::filesystem::directory_entry &entry);
		
		public: bool *GetInputTargetBool(size_t index) override;

		public: void Update(Core::UiBuilder &builder) override;

			private: void ProcessLatestIconInputs();
		
	};
	
	
}
