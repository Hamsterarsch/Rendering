#pragma once
#include "RendererExportHelper.hpp"
#include "Shared/InterfaceHelpers.hpp"
#include "Commands/CommandProcessor.hpp"


namespace Renderer{ class UsesReferences; }


namespace Renderer::Commands
{
	class RENDERER_DLLSPEC Command
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Command)

		
		public: virtual void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) = 0;
			
		public: virtual void Execute(CommandProcessor &context) = 0;

		
	};

	
}
