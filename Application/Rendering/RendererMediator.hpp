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
		
		private: size_t mainWindowSurface;

		private: SceneRenderer sceneRenderer;

		private: UiRenderer uiRenderer;

		private: unsigned char minimumFrameDeltaMs;
		
		private: size_t lastSubmitTime;



		public: RendererMediator(RendererFacade &renderer, SceneRenderer &&sceneRenderer, UiRenderer &&uiRenderer);

		
		public: void SubmitFrame();

			private: void SubmitCommand(UniquePtr<Commands::Command> &&command);

		public: ::Renderer::RendererFacade &Renderer() { return *underlyingRenderer; }

		public: Commands::CommandFactory &CommandFactory() { return *commandFactory; }

		public: void SetMainWindowSurface(ResourceHandle::t_hash surface);
		
	};

	
}
