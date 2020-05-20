#include "Commands/PresentSurfaceCommand.hpp"


namespace Renderer::DX12::Commands
{
	PresentSurfaceCommand::PresentSurfaceCommand(const RenderSurface &surface) :
		surface{ surface }
	{
	}


	
	void PresentSurfaceCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t)) {}


	
	void PresentSurfaceCommand::Execute(CommandProcessor &context)
	{
		context.SubmitAndWaitForGpuWork();
		surface.Present();
		
	}

	
}
