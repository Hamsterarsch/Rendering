#pragma once
#include "Window/Windows/Window.hpp"
#include "RendererFacade.hpp"
#include "Rendering/RendererMediator.hpp"
#include "Ui/UiStateMachine.hpp"
#include "AssetPtr.hpp"


#include "Core/Version.hpp"


namespace App::Ui::Core
{
	class UiFrontend;
}


namespace App::Windows
{
	class Application
	{
		private: Window window;

		private: UniquePtr<Renderer::RendererFacade> renderer;

		private: Renderer::HandleWrapper mainWindowSurface;
		
		private: Rendering::RendererMediator rendererMediator;

		private: Core::Version programVersion;
		
		private: UniquePtr<assetSystem::AssetSystem> programAssets;

		private: UniquePtr<assetSystem::AssetSystem> projectAssets;
		
		private: Ui::UiStateMachine ui;


		
		public: static Application &Get();
		
			private: Application();

		
		public: assetSystem::AssetSystem &GetProgramAssets() { return *programAssets; }

		public: assetSystem::AssetSystem &GetProjectAssets() { return *projectAssets; }

		public: Renderer::RendererFacade &GetRenderer() { return *renderer; }

		public: Core::Version GetProgramVersion() const { return programVersion; }
		
		public: bool ProjectAssetsAreInvalid() const { return projectAssets == nullptr; }

		public: void SetProjectAssets(UniquePtr<assetSystem::AssetSystem> &&assets);
		
		
		public: void EnterLoop();

			private: void Update();

		public: void ResizeMainWindow(int width, int height);



		
	};

	
}
