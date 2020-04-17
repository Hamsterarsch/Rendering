#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DX12/CmdList.hpp"
#include "Shared/PtrTypes.hpp"


namespace Renderer
{
	namespace DX12
	{
		class ResourceRegistryReadOnly;
		class ResourceRegistry;
		
		class RenderCommand
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS_NOCTOR(RenderCommand)


			private: size_t signatureHandle;

			private: size_t psoHandle;


			public: RenderCommand(size_t signatureHandle, size_t psoHandle) :
				signatureHandle{ signatureHandle },
				psoHandle{ psoHandle }
			{				
			}
			
			public: inline size_t GetSignatureHandle() const { return signatureHandle; }
			
			public: inline size_t GetPsoHandle() const { return psoHandle; }
								
			public: virtual void ExecuteOperationOnResourceReferences(ResourceRegistry *registry, void(ResourceRegistry:: *operation)(size_t)) = 0;
			
			public: virtual void Record(RHA::DX12::CmdList *list, ResourceRegistryReadOnly &registry) = 0;
			
		};

		
	}

	
}
