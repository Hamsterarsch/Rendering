#include "Rendering/RendererMediator.hpp"
#include "RendererFacade.hpp"


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
		mainDepthTextureView{ 0 },
		sceneRenderer{ std::move(sceneRenderer) },
		uiRenderer{ std::move(uiRenderer) },		
		submitIterations{ 0 },
		frameCounter{ underlyingRenderer->GetCounterFactory().MakeCounter(0) },
		maximumFrameLag{ 3 }
	{		
	}


	
	void RendererMediator::SubmitFrame(const UniquePtr<GraphVisitorHarvestMeshes> &sceneGraphData)
	{
		if(not mainWindowSurface || not mainDepthTextureView)
		{
			return;
		}

		if(submitIterations >= maximumFrameLag)
		{
			underlyingRenderer->GetCounterFactory().WaitForCounterToReach(frameCounter, submitIterations - maximumFrameLag);
			
		}
						
		SubmitCommand(commandFactory->PrepareSurfaceForRendering(mainWindowSurface));//todo does surface binding persist across cmd lists		
		SubmitCommand(commandFactory->ClearDepthTexture(mainDepthTextureView));
				
		sceneRenderer.SubmitFrame(sceneGraphData);		
		//uiRenderer.SubmitFrame(); todo: bind rts inside the ui submit code
		
		
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


	
	void RendererMediator::SetMainWindowSurface(const ResourceHandle::t_hash surface)
	{
		mainWindowSurface = surface;
		
	}


	
	void RendererMediator::SetMainDepthTextureView(const ResourceHandle::t_hash depthTextureView)
	{
		mainDepthTextureView = depthTextureView;
		
	}


	
	void RendererMediator::OnMainWindowSurfaceSizeChanged(const Math::VectorUint2 &newSize)
	{
		sceneRenderer.OnMainWindowSurfaceSizeChanged(newSize);
		
	}

	
}
