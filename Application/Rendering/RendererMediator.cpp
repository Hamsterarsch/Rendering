#include "Rendering/RendererMediator.hpp"
#include "Renderer.hpp"
#include <chrono>


namespace App::Rendering
{
	RendererMediator::RendererMediator
	(
		Renderer::HandleWrapper &&mainWindowSurface,
		Renderer::Renderer &renderer,
		SceneRenderer &&sceneRenderer
	)	:
		underlyingRenderer{ &renderer },
		commandFactory{ renderer.MakeCommandFactory() },
		mainWindowSurface{ std::move(mainWindowSurface) },
		sceneRenderer{ std::move(sceneRenderer) },
		minimumFrameDeltaMs{ 1 },
		lastSubmitTime{ 0 }
	{		
	}
	
	
	void RendererMediator::SubmitFrame()
	{
		const auto currentTime{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() };
		const auto delta{ currentTime - lastSubmitTime };
		lastSubmitTime = currentTime;
		if(delta <= minimumFrameDeltaMs)
		{
			return;
		}
		
		SubmitCommand(commandFactory->PrepareSurfaceForRendering(mainWindowSurface));
		
		sceneRenderer.SubmitFrame();
		
		SubmitCommand(commandFactory->PresentSurface(mainWindowSurface));

		underlyingRenderer->DestroyUnreferencedResources();
		underlyingRenderer->DestroyExecutedCommands();
				
	}
	
			void RendererMediator::SubmitCommand(UniquePtr<Renderer::Commands::Command> &&command)
			{
				underlyingRenderer->SubmitCommand(std::move(command));
				
			}
			
	
}
