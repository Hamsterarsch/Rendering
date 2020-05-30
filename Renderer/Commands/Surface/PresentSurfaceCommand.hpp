#pragma once
#include "Commands/DX12Command.hpp"
#include "DX12/WindowSurface.hpp"

namespace Renderer::DX12::Commands
{
	class PresentSurfaceCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash surface;


		
		public: PresentSurfaceCommand(const ResourceHandle::t_hash surface) : surface{ surface } {}
		
		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t))	override
		{
			Invoke(registry, operation, surface);
			
		}
		
		public: void Execute(DX12CommandProcessor &context) override
		{			
			context.GetRegistry().GetSurface(surface)->RecordPreparationForPresenting(context.GetList());
			context.SubmitAndWaitForGpuWork();
			context.GetRegistry().GetSurface(surface)->Present();
			
		}
		
	};
	
	
}



