#include "Rendering/RendererMediator.hpp"
#include "RendererFacade.hpp"
#include <chrono>
#include <iostream>


namespace App::Rendering
{
	RendererMediator::RendererMediator
	(		
		RendererFacade &renderer,
		SceneRenderer &&sceneRenderer,
		UiRenderer &&uiRenderer
	)	:
		underlyingRenderer{ &renderer },
		commandFactory{ renderer.MakeCommandFactory() },
		mainWindowSurface{ 0 },
		sceneRenderer{ std::move(sceneRenderer) },
		uiRenderer{ std::move(uiRenderer) },
		minimumFrameDeltaMs{ 2 },
		lastSubmitTime{ 0 }
	{		
	}


	
	void RendererMediator::SubmitFrame()
	{
		if(!mainWindowSurface)
		{
			return;
		}
		
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

	void RendererMediator::SetMainWindowSurface(ResourceHandle::t_hash surface)
	{
		mainWindowSurface = surface;
		
	}
	
}
