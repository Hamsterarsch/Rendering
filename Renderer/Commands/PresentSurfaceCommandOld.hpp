#pragma once
#include "Commands/DX12Command.hpp"
#include "Resources/RenderSurface.hpp"


namespace Renderer::DX12::Commands
{
	class PresentSurfaceCommandOld : public DX12Command
	{
		public: RenderSurface surface;

		

		public: PresentSurfaceCommandOld(const RenderSurface &surface);
		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t)) override;

		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
}
