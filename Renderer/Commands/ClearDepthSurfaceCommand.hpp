#pragma once
#include "Commands/DX12Command.hpp"
#include "Resources/RenderSurface.hpp"


namespace Renderer::DX12::Commands
{
	class ClearDepthSurfaceCommand final : public DX12Command
	{
		private: RenderSurface surface;


		
		public: ClearDepthSurfaceCommand(const RenderSurface &surface) : surface{ surface } {}

		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t))	override {}
		
		public: void Execute(DX12CommandProcessor &context) override
		{
			surface.RecordClearDepthSurface(context.GetList());
			
		}
		
	};
	
	
}
