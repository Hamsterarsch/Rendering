#pragma once
#include "Commands/RenderCommandCompute.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"


namespace Renderer::DX12
{
	class CommandAssignLightsToTiles : public RenderCommandCompute
	{
		private: size_t globalLightListHandle;

		private: size_t gridAABBHandle;

		private: size_t activeTileListHandle;

		private: size_t tileGridDataHandle;

		private: HandleWrapper relevantLightIndices, offsetsToRelevantLightChunks;

		private: DescriptorAllocator descAlloc;

		private: size_t dispatchSize;

		

		public: CommandAssignLightsToTiles
		(
			size_t signatureHandle,
			size_t psoHandle,
			size_t activeTileListHandle,
			size_t globalLightListHandle,
			size_t gridAABBHandle,			
			size_t tileGridDataHandle,
			size_t lightCount,
			size_t tileCount,
			MaintainsInternalRenderResources &factory, 
			ResourceRegistry &registry, 
			DescriptorMemory &descMem	
		);

		
		public: void ExecuteOperationOnResourceReferences(UsesReferences *registry, void( UsesReferences:: *operation)(size_t)) override;
		
		public: void Record(RHA::DX12::CmdList *list, HasQueriableResources &registry) override;
		
	};
	
	
}
