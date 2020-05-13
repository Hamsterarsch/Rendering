#include "Commands/CommandFlagActiveVolumeTiles.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Commands/RenderMeshCommand.hpp"


namespace Renderer
{
	namespace DX12
	{
		CommandFlagActiveVolumeTiles::CommandFlagActiveVolumeTiles
		(
			const size_t signatureHandle,
			const size_t psoHandle,
			const size_t volumeTileGridDataBufferHandle,
			const size_t volumeTileCount,
			MaintainsInternalRenderResources &factory,
			ResourceRegistry &registry, 
			DescriptorMemory &descMem
		) :
			RenderCommandGraphics{ signatureHandle, psoHandle },
			tileFlagsSizeInBytes{ volumeTileCount * sizeof(decltype(tileData)::value_type) },
			readbackBuffer{ factory.MakeReadbackBuffer(tileFlagsSizeInBytes) },
			tileData(volumeTileCount, 0),
			flagBufferHandle{ &factory, factory.MakeUavBuffer(tileData.data(), tileFlagsSizeInBytes) },
			volumeTileDataBuffer{ volumeTileGridDataBufferHandle },
			descAlloc{ descMem.GetDescriptorAllocator(2,0) }
		{
			descAlloc.OpenNewTable();
			descAlloc.CreateUavBuffer
			(
				registry.GetResource(flagBufferHandle),
				registry.GetSignatureUavOffset(GetSignatureHandle(), 1),
				0,
				volumeTileCount,
				sizeof(decltype(tileData)::value_type)
			);

			auto *resource{ registry.GetResource(volumeTileDataBuffer) };
			auto desc{ resource->GetDesc() };
						
			descAlloc.CreateCbv
			(
				resource,
				registry.GetSignatureCbvOffset(GetSignatureHandle(), 1),
				desc.Width
			);
			
		}

		

		void CommandFlagActiveVolumeTiles::ExecutePostGpuWork()
		{
			void *readbackData;
			auto mapResult{ readbackBuffer->Map(0, nullptr, &readbackData) };

			memcpy(tileData.data(), readbackData, tileFlagsSizeInBytes);

			D3D12_RANGE range{0,0};
			readbackBuffer->Unmap(0, &range);


			size_t activeTiles{ 0 };
			for(size_t tileIndex{ 0 }; tileIndex < tileData.size(); ++tileIndex)
			{
				if(tileData[tileIndex] > 0)
				{
					tileData[activeTiles] = tileIndex;
					++activeTiles;
				}
			}
			tileData.resize(activeTiles);
						
			
		}

		

		void CommandFlagActiveVolumeTiles::ExecuteOperationOnResourceReferences(UsesReferences *registry, void( UsesReferences:: *const operation)(size_t))
		{
			(registry->*operation)(volumeTileDataBuffer);
			(registry->*operation)(flagBufferHandle);

			for(auto &&cmd : meshCommands)
			{
				cmd.ExecuteOperationOnResourceReferences(registry, operation);
			}			
			
		}


		
		void CommandFlagActiveVolumeTiles::Record(RHA::DX12::CmdList *list, HasQueriableResources &registry)
		{										
			list->RecordSetGraphicsSignatureTable(2, descAlloc.GetCurrentTableStartForView());

			for(auto &&cmd : meshCommands)
			{
				cmd.Record(list, registry);
			}			
							
			list->RecordBarrierTransition(registry.GetResource(flagBufferHandle), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);								
			list->RecordCopyResource(readbackBuffer.Get(), registry.GetResource(flagBufferHandle));
			
		}

		

		void CommandFlagActiveVolumeTiles::AddRenderMeshCommand(const RenderMeshCommand &command)
		{
			meshCommands.push_back(command);
			
		}

		
	}

	
}
