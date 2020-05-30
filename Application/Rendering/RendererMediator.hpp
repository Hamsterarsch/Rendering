#pragma once
#include "Rendering/SceneRenderer.hpp"
#include "Commands/CommandFactory.hpp"
#include "Resources/HandleWrapper.hpp"
#include "UiRenderer.hpp"


namespace Renderer{ class Renderer; }


namespace App::Rendering
{
	class RendererMediator
	{
		private: ::Renderer::Renderer *underlyingRenderer;

		private: UniquePtr<Commands::CommandFactory> commandFactory;
		
		private: ::Renderer::HandleWrapper mainWindowSurface;

		private: SceneRenderer sceneRenderer;

		private: UiRenderer uiRenderer;

		private: unsigned char minimumFrameDeltaMs;
		
		private: size_t lastSubmitTime;



		public: RendererMediator(HandleWrapper &&mainWindowSurface, ::Renderer::Renderer &renderer, SceneRenderer &&sceneRenderer, UiRenderer &&uiRenderer);

		
		public: bool DidRenderLastSubmit() const;
		
		public: void SubmitFrame();

			private: void SubmitCommand(UniquePtr<Commands::Command> &&command);

		public: ::Renderer::Renderer &Renderer() { return *underlyingRenderer; }

		public: Commands::CommandFactory &CommandFactory() { return *commandFactory; }
		
	};

	
}
