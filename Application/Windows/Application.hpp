#pragma once
#include "Window/Windows/Window.hpp"
#include "Rendering/RendererMediator.hpp"
#include "AssetSystem.hpp"
#include "Core/Version.hpp"
#include "Ui/UiStateMachine.hpp"
#include "RendererFacade.hpp"
#include "AssetTypes/AssetTypesRegistry.hpp"
#include "Core/Application.hpp"
#include "Scene/SceneGraph.hpp"
#include "Ui/Core/UiBuilderImpl.hpp"


namespace App::Ui::User { class SceneEditorFrontend; }
namespace App::Ui::Core{ class UiFrontend; }


#include "AssetTypes/StaticMeshAsset.hpp"
#include "AssetPtr.hpp"
#include "Scene/GraphNode.hpp"
#include "Scene/ContentMesh.hpp"

#include "Ui/Misc/OutlinerInfo.hpp"

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

		private: Renderer::HandleWrapper mainDepthTexture;

		private: Renderer::HandleWrapper mainDepthTextureView;
		
		private: Assets::AssetTypesRegistry assetTypesRegistry;
		
		private: Rendering::RendererMediator rendererMediator;		
		
		private: Ui::UiStateMachine ui;

		private: Ui::Core::UiBuilderImpl builder;

		private: Scene::SceneGraph scene;

		private: Math::Vector3 currentCameraPos;

		private: Math::Vector3 currentCameraRot;

		private: static constexpr float cameraSpeed{ 1 };


		
		public: static Application &Get();
		
			private: Application();

				private: static UniquePtr<Renderer::RendererFacade> MakeRendererAndAddProgramShaderInclude(HWND window, assetSystem::AssetSystem &programAssets);

		public: ~Application();
		
		public: Application(Application &&other) = delete;

		public: Application &operator=(Application &&rhs) = delete;		
		
		public: Application(const Application &) = delete;

		public: Application &operator=(Application &) = delete;
		
		
		public: assetSystem::AssetSystem &GetProgramAssets() override { return *programAssets; }

		public: assetSystem::AssetSystem &GetProjectAssets() override { return *projectAssets; }

		public: Renderer::RendererFacade &GetRenderer() override { return *renderer; }

		public: Ui::UiStateMachine &GetUiStateMachine() override { return ui; }

		public: Core::Version GetProgramVersion() const override { return programVersion; }
		
		public: const Assets::AssetTypesRegistry &GetAssetTypes() const override { return assetTypesRegistry; }
		
		public: bool ProjectAssetsAreInvalid() const override { return projectAssets == nullptr; }

		public: Scene::SceneGraph &GetCurrentScene() override { return scene; }
		
		public: void SetProjectAssets(UniquePtr<assetSystem::AssetSystem> &&assets) override;

		
		public: void EnterLoop();

			private: void Update();
					 		
				private: void QueryUiInputAndSubmitUiRenderData();

				private: void ApplyCameraMovementInputs();

					private: bool ShouldUpdateCameraRot() const;

					private: bool ShouldUpdateCameraPos() const;

					private: static bool IsKeyDown(int KeyCode);

		public: void ResizeMainWindow(int width, int height);

	};

	
}
