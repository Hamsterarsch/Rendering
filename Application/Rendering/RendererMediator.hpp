#pragma once
#include "Rendering/SceneRenderer.hpp"
#include "Commands/CommandFactory.hpp"
#include "Resources/HandleWrapper.hpp"
#include "UiRenderer.hpp"


namespace Renderer{ class RendererFacade; }


namespace App::Rendering
{
	class RendererMediator
	{
		private: RendererFacade *underlyingRenderer;

		private: UniquePtr<Commands::CommandFactory> commandFactory;
		
		private: ResourceHandle::t_hash mainWindowSurface;

		private: ResourceHandle::t_hash mainDepthTextureView;

		private: SceneRenderer sceneRenderer;

		private: UiRenderer uiRenderer;

		private: size_t submitIterations;

		private: CounterFactory::CounterID frameCounter;

		private: unsigned char maximumFrameLag;



		public: RendererMediator() = default;
		
		public: RendererMediator(RendererFacade &renderer, SceneRenderer &&sceneRenderer, UiRenderer &&uiRenderer);

		
		public: void SubmitFrame(const UniquePtr<GraphVisitorHarvestMeshes> &sceneGraphData);

			private: void SubmitCommand(UniquePtr<Commands::Command> &&command);

		public: RendererFacade &Renderer() { return *underlyingRenderer; }

		public: Commands::CommandFactory &CommandFactory() { return *commandFactory; }

		public: ResourceHandle::t_hash GetMainDepthTextureView() const { return mainDepthTextureView; }

		public: ResourceHandle::t_hash GetMainWindowSurface() const { return mainWindowSurface; }

		public: void SetCurrentSceneView(const Math::Vector3 &newPos, const Math::Vector3 &newRot);
		
		public: void SetMainWindowSurface(ResourceHandle::t_hash surface);

		public: void SetMainDepthTextureView(ResourceHandle::t_hash depthTextureView);

		public: void OnMainWindowSurfaceSizeChanged(const Math::VectorUint2 &newSize);
		
	};

	
}
