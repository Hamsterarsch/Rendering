#pragma once
#include "Commands/RenderCommand.hpp"

#include "DX12/CmdList.hpp"
#include "Resources/HasQueriableResources.hpp"

namespace Renderer
{
	namespace DX12
	{
		class RenderCommandCompute : public RenderCommand
		{
			public: RenderCommandCompute(size_t signatureHandle, size_t psoHandle) : RenderCommand{ signatureHandle, psoHandle } {}
			
			public:	void RecordFixedCommandState(RHA::DX12::CmdList *list, HasQueriableResources &registry, size_t globalBufferHandle) const final override
			{
				if(PsoIsValid())
				{
					list->RecordSetPipelineState(registry.GetPso(GetPsoHandle()));					
				}

				if(SignatureIsValid())
				{
					list->RecordSetComputeSignature(registry.GetSignature(GetSignatureHandle()));					
				}
				list->RecordSetComputeSignatureCbv(GetGlobalBufferSlot(), registry.GetResourceGpuAddress(globalBufferHandle));
			
			}
			
			
		};
		
		
	}
	
	
}

