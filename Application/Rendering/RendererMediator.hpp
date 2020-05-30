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
		
		private: HandleWrapper mainWindowSurface;

		private: SceneRenderer sceneRenderer;

		private: UiRenderer uiRenderer;

		private: unsigned char minimumFrameDeltaMs;
		
		private: size_t lastSubmitTime;



		public: RendererMediator(HandleWrapper &&mainWindowSurface, RendererFacade &renderer, SceneRenderer &&sceneRenderer, UiRenderer &&uiRenderer);

		
		public: bool DidRenderLastSubmit() const;
		
		public: void SubmitFrame();

			private: void SubmitCommand(UniquePtr<Commands::Command> &&command);

		public: ::Renderer::RendererFacade &Renderer() { return *underlyingRenderer; }

		public: Commands::CommandFactory &CommandFactory() { return *commandFactory; }
		
	};

	
}
