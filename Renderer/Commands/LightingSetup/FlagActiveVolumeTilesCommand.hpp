#pragma once
#include "Commands/RenderCommandGraphics.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include "Commands/RenderMeshCommand.hpp"
#include <vector>


namespace Renderer
{
	class MaintainsInternalRenderResources;

	namespace DX12
	{
		class DescriptorMemory;
	}
}


namespace Renderer::DX12::Commands
{		
	class RenderMeshCommand;
	class GenerateActiveTileListCommand;
	
	class FlagActiveVolumeTilesCommand final : public RenderCommandGraphics
	{
		private: GenerateActiveTileListCommand *pass;
					


		private: DescriptorAllocator descAlloc;

		private: std::vector<RenderMeshCommand> meshCommands;
		
		private: static size_t signature;

		private: static size_t pso;

		public: static void SetPso(size_t pso) { FlagActiveVolumeTilesCommand::pso = pso; }

		public: static void SetSignature(size_t signature) { FlagActiveVolumeTilesCommand::signature = signature; }
		
		public: FlagActiveVolumeTilesCommand
		(			
			GenerateActiveTileListCommand &pass,
			MaintainsInternalRenderResources &factory, 
			ResourceRegistry &registry, 
			DescriptorMemory &descMem				
		);

		
		public: void ExecuteOperationOnResourceReferences( UsesReferences &registry, void( UsesReferences::* operation)(size_t)) override;
		
		public: void Execute(DX12CommandProcessor &context) override;

		public: void AddRenderMeshCommand(const RenderMeshArguments &command);


										
	};

	
}
