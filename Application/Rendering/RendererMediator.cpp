#include "Rendering/RendererMediator.hpp"
#include "Renderer.hpp"
#include <chrono>
#include <iostream>


namespace App::Rendering
{
	RendererMediator::RendererMediator
	(
		HandleWrapper &&mainWindowSurface,
		::Renderer::Renderer &renderer,
		SceneRenderer &&sceneRenderer,
		UiRenderer &&uiRenderer
	)	:
		underlyingRenderer{ &renderer },
		commandFactory{ renderer.MakeCommandFactory() },
		mainWindowSurface{ std::move(mainWindowSurface) },
		sceneRenderer{ std::move(sceneRenderer) },
		uiRenderer{ std::move(uiRenderer) },
		minimumFrameDeltaMs{ 2 },
		lastSubmitTime{ 0 }
	{		
	}


	
	void RendererMediator::SubmitFrame()
	{
		/*
		const auto currentTime{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() };
		const auto delta{ currentTime - lastSubmitTime };
		lastSubmitTime = currentTime;
		if(delta <= minimumFrameDeltaMs)
		{
			std::cout << "skip render" << std::endl; 
			return;
		} todo do something against idle submission stacking */
				
		SubmitCommand(commandFactory->PrepareSurfaceForRendering(mainWindowSurface));
		
		sceneRenderer.SubmitFrame();		
		uiRenderer.SubmitFrame();
		//are scissors reset automatically ?
		SubmitCommand(commandFactory->PresentSurface(mainWindowSurface));

		underlyingRenderer->DestroyUnreferencedResources();
		underlyingRenderer->DestroyExecutedCommands();
				
	}
	
			void RendererMediator::SubmitCommand(UniquePtr<Commands::Command> &&command)
			{
				underlyingRenderer->SubmitCommand(std::move(command));
				
			}
			
	
}
