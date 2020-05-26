#pragma once
#include "Rendering/SceneRenderer.hpp"
#include "Commands/CommandFactory.hpp"
#include "Resources/HandleWrapper.hpp"


namespace Renderer{ class Renderer; }


namespace App::Rendering
{
	class RendererMediator
	{
		private: Renderer::Renderer *underlyingRenderer;

		private: UniquePtr<Renderer::Commands::CommandFactory> commandFactory;
		
		private: Renderer::HandleWrapper mainWindowSurface;

		private: SceneRenderer sceneRenderer;

		private: unsigned char minimumFrameDeltaMs;
		
		private: size_t lastSubmitTime;



		public: RendererMediator(Renderer::HandleWrapper &&mainWindowSurface, Renderer::Renderer &renderer, SceneRenderer &&sceneRenderer);

		
		public: void SubmitFrame();

				private: void SubmitCommand(UniquePtr<Renderer::Commands::Command> &&command);
		
	};

	
}
