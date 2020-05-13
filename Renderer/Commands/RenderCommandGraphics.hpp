#pragma once
#include "Commands/RenderCommand.hpp"

#include "DX12/CmdList.hpp"
#include "Resources/HasQueriableResources.hpp"

namespace Renderer
{
	namespace DX12
	{
		class RenderCommandGraphics : public RenderCommand
		{
			public: RenderCommandGraphics(size_t signatureHandle, size_t psoHandle) : RenderCommand{ signatureHandle, psoHandle } {}
			
			public:	void RecordFixedCommandState(RHA::DX12::CmdList *list, HasQueriableResources &registry, size_t globalBufferHandle) const final override
			{
				if(PsoIsValid())
				{
					list->RecordSetPipelineState(registry.GetPso(GetPsoHandle()));					
				}

				if(SignatureIsValid())
				{				
					list->RecordSetGraphicsSignature(registry.GetSignature(GetSignatureHandle()));
				}
				
				list->RecordSetGraphicsSignatureCbv(GetGlobalBufferSlot(), registry.GetResourceGpuAddress(globalBufferHandle));
			
			}
			
			
		};
		
		
	}
	
	
}
