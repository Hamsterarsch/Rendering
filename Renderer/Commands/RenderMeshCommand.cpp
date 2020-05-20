#include <d3d12.h>
#include "DX12/CmdList.hpp"
#include "RenderMeshCommand.hpp"
#include "Resources/ResourceRegistry.hpp"


namespace Renderer::DX12::Commands
{
	RenderMeshCommand::RenderMeshCommand(const RenderMeshArguments &arguments) :
		RenderCommandGraphics{ arguments.signature, arguments.pso },
		meshHandle{ arguments.mesh },
		byteOffsetToIndexData{ arguments.byteOffsetToIndexData },
		indicesSizeInBytes{ arguments.indicesSizeInBytes },
		transformBufferHandle{ arguments.instanceTransforms },
		instanceCount{ arguments.instanceCount },
		gridDataBuffer{ 0 },
		lightBuffer{ 0 },
		relevantLightsList{ 0 },
		offsetsToRelevantLightsList{ 0 }
	{
	}



	RenderMeshCommand::RenderMeshCommand
	(
		const size_t signature,
		const size_t pso,
		const RenderMeshArguments &arguments
	)	:
		RenderCommandGraphics{ signature, pso},
		meshHandle{ arguments.mesh },
		byteOffsetToIndexData{ arguments.byteOffsetToIndexData },
		indicesSizeInBytes{ arguments.indicesSizeInBytes },
		transformBufferHandle{ arguments.instanceTransforms },
		instanceCount{ arguments.instanceCount },
		gridDataBuffer{ 0 },
		lightBuffer{ 0 },
		relevantLightsList{ 0 },
		offsetsToRelevantLightsList{ 0 }
	{
	}



	RenderMeshCommand::RenderMeshCommand
	(
		const size_t signature,
		const size_t pso,
		const size_t gridDataBuffer,
		const size_t lightsBuffer, 
		const size_t relevantLightsList,
		const size_t offsetToRelevantLightsList,
		const RenderMeshArguments &arguments,
		DescriptorAllocator &filledDescAlloc
	)	:
		RenderCommandGraphics{ signature, pso },
		meshHandle{ arguments.mesh },
		byteOffsetToIndexData{ arguments.byteOffsetToIndexData },
		indicesSizeInBytes{ arguments.indicesSizeInBytes },
		transformBufferHandle{ arguments.instanceTransforms },
		instanceCount{ arguments.instanceCount },
		gridDataBuffer{ gridDataBuffer },
		lightBuffer{ lightsBuffer },
		relevantLightsList{ relevantLightsList },
		offsetsToRelevantLightsList{ offsetToRelevantLightsList },
		descAlloc{ &filledDescAlloc }
	{
	}


	void RenderMeshCommand::ExecuteOperationOnResourceReferences
	(
		UsesReferences &registry,
		void(UsesReferences:: *operation)(size_t)
	)
	{
		RenderCommandGraphics::ExecuteOperationOnResourceReferences(registry, operation);
		
		(registry.*operation)(meshHandle);
		(registry.*operation)(transformBufferHandle);							

		(registry.*operation)(gridDataBuffer);
		(registry.*operation)(lightBuffer);
		(registry.*operation)(relevantLightsList);
		(registry.*operation)(offsetsToRelevantLightsList);
		
	}

	void RenderMeshCommand::Execute
	(
		CommandProcessor &context
	)
	{
		RenderCommandGraphics::Execute(context);
		
		auto &list{ context.GetList() };
		auto &registry{ context.GetRegistry() };
		
		views.vertexView.BufferLocation = registry.GetResourceGpuAddress(meshHandle);
		views.vertexView.SizeInBytes = byteOffsetToIndexData;
		views.vertexView.StrideInBytes = vertexStride;

		views.indexView.BufferLocation = views.vertexView.BufferLocation + byteOffsetToIndexData;
		views.indexView.Format = DXGI_FORMAT_R32_UINT;
		views.indexView.SizeInBytes = indicesSizeInBytes;


		auto g{ list.AsGraphicsList() };

		g->IASetVertexBuffers(0, 1, &views.vertexView);
		g->IASetIndexBuffer(&views.indexView);
		g->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if(transformBufferHandle > 0)
		{
			g->SetGraphicsRootConstantBufferView(1, registry.GetResourceGpuAddress(transformBufferHandle));				
		}

				
		g->DrawIndexedInstanced(indicesSizeInBytes / sizeof(unsigned), instanceCount, 0, 0, 0);
		
	}

	
}