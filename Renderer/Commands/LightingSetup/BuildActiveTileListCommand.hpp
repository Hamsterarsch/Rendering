#pragma once
#include "Commands/RenderCommandCompute.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"


namespace Renderer
{
	class MaintainsInternalRenderResources;
}


namespace Renderer::DX12::Commands
{
	class GenerateActiveTileListCommand;
	
	class BuildActiveTileListCommand final : public RenderCommandCompute
	{
		private: GenerateActiveTileListCommand *pass;
		
		private: HandleWrapper tileCountBuffer;

		private: DescriptorAllocator descAlloc;

		private: size_t groupsToDispatch;



		static size_t signature;

		static size_t pso;

		public: static void SetPso(size_t pso) { BuildActiveTileListCommand::pso = pso; }

		public: static void SetSignature(size_t signature) { BuildActiveTileListCommand::signature = signature; }

		
		public:	BuildActiveTileListCommand
		(		
			GenerateActiveTileListCommand &pass,
			MaintainsInternalRenderResources &factory, 
			ResourceRegistry &registry, 
			DescriptorMemory &descMem	
		);

		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t)) override;
				
		public: void Execute(CommandProcessor &context) override;
		
	};
	
	
}

