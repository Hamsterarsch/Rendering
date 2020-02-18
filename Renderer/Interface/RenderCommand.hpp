#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DX12/CmdList.hpp"
#include "Shared/PtrTypes.hpp"


namespace Renderer
{
	namespace DX12
	{
		class ResourceRegistry;
		
		class RenderCommand
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(RenderCommand)

			
			public: virtual size_t GetPsoHandle() const = 0;

			public: virtual size_t GetSignatureHandle() const = 0;
								
			public: virtual void ExecuteOperationOnResourceReferences(ResourceRegistry *registry, void(ResourceRegistry:: *operation)(size_t)) = 0;
			
			public: virtual void Record(RHA::DX12::CmdList *list, ResourceRegistry &registry) = 0;
			
		};

		
	}

	
}
