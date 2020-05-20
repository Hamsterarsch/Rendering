#pragma once
#include "Commands/LightingSetup/FlagActiveVolumeTilesCommand.hpp"
#include "Commands/LightingSetup/BuildActiveTileListCommand.hpp"


namespace Renderer::DX12::Commands
{
	class GenerateActiveTileListCommand final : public Command
	{		
		public: size_t lightCount;		
		
		public: size_t lightGridTileCount;
		
		public: size_t lightGridBoundingBoxes;

		public: size_t lightsList;

		public: size_t lightGridData;		
		
		public: HandleWrapper tileFlags;

		public: HandleWrapper activeTileList;
		
		public: size_t activeTileCount;

		public: FlagActiveVolumeTilesCommand flagTilesCommand;

		public: BuildActiveTileListCommand buildActiveTileListCommand;

		private: DxPtr<ID3D12Resource> activeTileCountReadback;

		
		
		public: GenerateActiveTileListCommand
		(
			size_t lightCount,
			size_t lightGridTileCount,
			size_t lightGridBoundingBoxes,
			size_t lightsList,
			size_t lightGridData,
			MaintainsInternalRenderResources &factory,
			ResourceRegistry &registry, 
			DescriptorMemory &descMem
		);


		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t)) override
		{			
			(registry.*operation)(lightGridBoundingBoxes);
			(registry.*operation)(lightsList);
			(registry.*operation)(lightGridData);

			(registry.*operation)(tileFlags);
			(registry.*operation)(activeTileList);
			
		}

		public: void Execute(CommandProcessor &context) override;

		
		public: void AddRenderMeshCommand(const RenderMeshArguments &command);
		
		
	};


}

