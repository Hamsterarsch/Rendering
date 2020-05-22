#include "Commands/DX12CommandFactory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Commands/Surface/PrepareSurfaceForRenderingCommand.hpp"
#include "Commands/Surface/PresentSurfaceCommand.hpp"


namespace Renderer::DX12::Commands
{
	DX12CommandFactory::DX12CommandFactory(ResourceRegistry &registry) :
		registry{ &registry }
	{
	}
		

	UniquePtr<Renderer::Commands::Command> DX12CommandFactory::PrepareSurfaceForRendering(const ResourceHandle::t_hash surface)
	{		
		return MakeUnique<PrepareSurfaceForRenderingCommand>(surface);
		
	}

	UniquePtr<Renderer::Commands::Command> DX12CommandFactory::PresentSurface(const ResourceHandle::t_hash surface)
	{
		return MakeUnique<PresentSurfaceCommand>(surface);
		
	}


	
}
