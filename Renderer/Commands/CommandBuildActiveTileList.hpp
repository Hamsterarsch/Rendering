#pragma once
#include "Commands/RenderCommandCompute.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"


namespace Renderer
{
	class MaintainsInternalRenderResources;
}


namespace Renderer::DX12
{
	class CommandBuildActiveTileList final : public RenderCommandCompute
	{
		private: size_t tileFlagBufferHandle;

		private: HandleWrapper tileCountBuffer;

		private: HandleWrapper activeTileListBuffer;
		
		private: DescriptorAllocator descAlloc;

		private: size_t groupsToDispatch;
				 		

		
		public:	CommandBuildActiveTileList
		(
			size_t signatureHandle, 
			size_t psoHandle,
			size_t tileFlagBufferHandle, 
			size_t volumeTileCount,
			MaintainsInternalRenderResources &factory, 
			ResourceRegistry &registry, 
			DescriptorMemory &descMem	
		);

		void ExecuteOperationOnResourceReferences(UsesReferences *registry, void( UsesReferences:: *operation)(size_t)) override;
				
		void Record(RHA::DX12::CmdList *list, HasQueriableResources &registry) override;

		
	};
	
	
}

