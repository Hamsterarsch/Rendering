#pragma once
#include "RenderCommandGraphics.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"
#include <vector>


namespace Renderer
{
	class MaintainsInternalRenderResources;
	
	namespace DX12
	{
		class DescriptorMemory;
		class RenderMeshCommand;
		
		class CommandFlagActiveVolumeTiles final : public RenderCommandGraphics
		{
			private: size_t tileFlagsSizeInBytes;

			private: DxPtr<ID3D12Resource> readbackBuffer;

			private: std::vector<unsigned> tileData;
			
			private: HandleWrapper flagBufferHandle;
			
			private: size_t  volumeTileDataBuffer;

			private: DescriptorAllocator descAlloc;

			private: std::vector<RenderMeshCommand> meshCommands;
			

			public: CommandFlagActiveVolumeTiles
			(
				size_t signatureHandle, 
				size_t psoHandle,
				size_t volumeTileGridDataBufferHandle,
				size_t volumeTileCount,
				MaintainsInternalRenderResources &factory, 
				ResourceRegistry &registry, 
				DescriptorMemory &descMem				
			);

			CommandFlagActiveVolumeTiles(const CommandFlagActiveVolumeTiles &) = delete;

			CommandFlagActiveVolumeTiles &operator=(const CommandFlagActiveVolumeTiles &) = delete;
			
			public: void ExecutePostGpuWork() override;
			
			public: void ExecuteOperationOnResourceReferences( UsesReferences* registry, void( UsesReferences::* operation)(size_t)) override;
			
			public: void Record(RHA::DX12::CmdList* list, HasQueriableResources& registry) override;

			public: void AddRenderMeshCommand(const RenderMeshCommand &command);
											
		};

		
	}

	
}
