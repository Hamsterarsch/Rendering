#include "Commands/PresentSurfaceCommandOld.hpp"


namespace Renderer::DX12::Commands
{
	PresentSurfaceCommandOld::PresentSurfaceCommandOld(const RenderSurface &surface) :
		surface{ surface }
	{
	}


	
	void PresentSurfaceCommandOld::ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t)) {}


	
	void PresentSurfaceCommandOld::Execute(DX12CommandProcessor &context)
	{
		context.SubmitAndWaitForGpuWork();
		surface.Present();
		
	}

	
}
