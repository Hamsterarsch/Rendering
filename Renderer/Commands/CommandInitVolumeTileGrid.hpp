#pragma once
#include "Commands/RenderCommandCompute.hpp"


namespace Renderer
{
	namespace DX12
	{
		class CommandInitVolumeTileGrid : public RenderCommandCompute
		{
			CommandInitVolumeTileGrid(size_t signatureHandle, size_t psoHandle) : RenderCommandCompute{signatureHandle, psoHandle}
			{
				
			}


			public:	void ExecuteOperationOnResourceReferences(UsesReferences* registry,	void( UsesReferences::* operation)(size_t)) override;
			
			void Record(RHA::DX12::CmdList* list, HasQueriableResources& registry) override;

			
		};

		
	}

	
}
