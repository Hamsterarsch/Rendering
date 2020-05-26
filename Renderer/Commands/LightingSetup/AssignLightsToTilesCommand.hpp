#pragma once
#include "Commands/RenderCommandCompute.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"

#include "Commands/LightingSetup/GenerateActiveTileListCommand.hpp"

namespace Renderer::DX12::Commands
{
	class AssignLightsToTilesCommand final : public RenderCommandCompute
	{
		private: size_t globalLightList;

		private: size_t tileGridBoundingBoxes;

		private: size_t activeTileList;

		private: size_t tileGridData;

		private: HandleWrapper relevantLightIndices, mappingToRelevantLights;

		private: DescriptorAllocator descAlloc;

		private: size_t dispatchSize;



		private: static size_t signature;

		private: static size_t pso;

		public: static void SetPso(size_t pso) { AssignLightsToTilesCommand::pso = pso; }

		public: static void SetSignature(size_t signature) { AssignLightsToTilesCommand::signature = signature; }
		

		public: AssignLightsToTilesCommand
		(
			size_t activeTileList,
			size_t globalLightList,
			size_t tileGridBoundingBoxes,			
			size_t tileGridData,
			size_t lightCount,
			size_t tileCount,
			size_t activeTileCount,
			MaintainsInternalRenderResources &factory, 
			ResourceRegistry &registry, 
			DescriptorMemory &descMem	
		);

		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;

		public: size_t GetRelevantLightIndexList() const { return relevantLightIndices; }

		public: size_t GetMappingToRelevantLights() const { return mappingToRelevantLights; }
		
	};
	
	
}
