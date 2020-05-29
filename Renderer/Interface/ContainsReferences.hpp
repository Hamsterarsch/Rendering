#pragma once
#include "RendererExportHelper.hpp"
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class UsesReferences;
	
	class RENDERER_DLLSPEC ContainsReferences
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(ContainsReferences)

		
		public: virtual void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) = 0;
		
	};

	
}

