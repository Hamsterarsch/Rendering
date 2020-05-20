#pragma once
#include "Commands/Command.hpp"
#include "Resources/RenderSurface.hpp"


namespace Renderer::DX12::Commands
{
	class ClearDepthSurfaceCommand final : public Command
	{
		private: RenderSurface surface;


		
		public: ClearDepthSurfaceCommand(const RenderSurface &surface) : surface{ surface } {}

		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t))	override {}
		
		public: void Execute(CommandProcessor& context) override
		{
			surface.RecordClearDepthSurface(context.GetList());
			
		}
		
	};
	
	
}
