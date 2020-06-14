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
		submitIterations{ 0 },
		frameCounter{ underlyingRenderer->GetCounterFactory().MakeCounter(0) },
		maximumFrameLag{ 3 }
	{		
	}


	
	void RendererMediator::SubmitFrame()
	{
		if(!mainWindowSurface)
		{
			return;
		}

		if(submitIterations >= maximumFrameLag)
		{
			underlyingRenderer->GetCounterFactory().WaitForCounterToReach(frameCounter, submitIterations - maximumFrameLag);
			
		}
						
		SubmitCommand(commandFactory->PrepareSurfaceForRendering(mainWindowSurface));
		
		sceneRenderer.SubmitFrame();		
		uiRenderer.SubmitFrame();
		
		//are scissors reset automatically ?
		SubmitCommand(commandFactory->PresentSurface(mainWindowSurface));
		SubmitCommand(commandFactory->IncreaseCounter(frameCounter, 1));
		
		underlyingRenderer->DestroyUnreferencedResources();
		underlyingRenderer->DestroyExecutedCommands();
		++submitIterations;
				
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
