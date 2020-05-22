#pragma once
#include "Commands/DX12Command.hpp"
#include "DX12/WindowSurface.hpp"

namespace Renderer::DX12::Commands
{
	class PrepareSurfaceForRenderingCommand final : public DX12Command
	{
		private: ResourceHandle::t_hash surface;


		
		public: PrepareSurfaceForRenderingCommand(const ResourceHandle::t_hash surface) : surface{ surface } {}
		
		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t))	override
		{
			registry.AddReference(surface);
			
		}
		
		public: void Execute(DX12CommandProcessor &context) override
		{			
			context.GetRegistry().GetSurface(surface)->RecordPreparationForRendering(context.GetList());
			
		}
		
	};
	
	
}
