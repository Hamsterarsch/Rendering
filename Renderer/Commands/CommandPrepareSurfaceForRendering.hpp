#pragma once


#include "RenderCommand.hpp"
#include "Resources/RenderSurface.hpp"

namespace Renderer::DX12
{
	class RenderSurface;

	class CommandPrepareSurfaceForRendering final : public RenderCommand
	{
		private: RenderSurface surface;


		
		public: CommandPrepareSurfaceForRendering(const RenderSurface &surface) : surface{ surface } {}
		
		public:	void RecordFixedCommandState
		(
			RHA::DX12::CmdList *list, 
			HasQueriableResources &registry,
			size_t globalBufferHandle
		) const override
		{			
		}
		
		public: void ExecuteOperationOnResourceReferences(UsesReferences *registry, void( UsesReferences:: *operation)(size_t))	override {}
		
		public: void Record(RHA::DX12::CmdList *list, HasQueriableResources &registry) override
		{
			surface.RecordSurfacePreparations(*list);
			
		}

		
	};
	
	
}
