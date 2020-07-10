#pragma once
#include "Window/Windows/Window.hpp"
#include "Rendering/RendererMediator.hpp"
#include "AssetSystem.hpp"
#include "Core/Version.hpp"
#include "Ui/UiStateMachine.hpp"
#include "RendererFacade.hpp"
#include "AssetTypes/AssetTypesRegistry.hpp"
#include "Core/Application.hpp"


namespace App::Ui::Core{ class UiFrontend; }


namespace App::Windows
{
	class Application final : public Core::Application
	{
		private: Core::Version programVersion;
		
		private: UniquePtr<assetSystem::AssetSystem> programAssets;

		private: UniquePtr<assetSystem::AssetSystem> projectAssets;
		
		private: Window window;
		
		private: UniquePtr<Renderer::RendererFacade> renderer;

		private: Renderer::HandleWrapper mainWindowSurface;
		
		private: Assets::AssetTypesRegistry assetTypesRegistry;
		
		private: Rendering::RendererMediator rendererMediator;		
		
		private: Ui::UiStateMachine ui;



		
		public: static Application &Get();
		
			private: Application();

				private: static UniquePtr<Renderer::RendererFacade> MakeRendererAndAddProgramShaderInclude(HWND window, assetSystem::AssetSystem &programAssets);

		
		public: assetSystem::AssetSystem &GetProgramAssets() override { return *programAssets; }

		public: assetSystem::AssetSystem &GetProjectAssets() override { return *projectAssets; }

		public: Renderer::RendererFacade &GetRenderer() override { return *renderer; }

		public: Ui::UiStateMachine &GetUiStateMachine() override { return ui; }

		public: Core::Version GetProgramVersion() const override { return programVersion; }
		
		public: const Assets::AssetTypesRegistry &GetAssetTypes() const override { return assetTypesRegistry; }
		
		public: bool ProjectAssetsAreInvalid() const override { return projectAssets == nullptr; }

		public: void SetProjectAssets(UniquePtr<assetSystem::AssetSystem> &&assets) override;

				
		public: void EnterLoop();

			private: void Update();

		public: void ResizeMainWindow(int width, int height);
	
	};

	
}
