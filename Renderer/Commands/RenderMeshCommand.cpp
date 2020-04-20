#include <d3d12.h>
#include "DX12/CmdList.hpp"
#include "RenderMeshCommand.hpp"
#include "Resources/ResourceRegistry.hpp"


namespace Renderer
{
	namespace DX12
	{


		RenderMeshCommand::RenderMeshCommand
		(
			const size_t signatureHandle, 
			const size_t psoHandle, 
			const size_t meshHandle, 
			const size_t byteOffsetToIndexData, 
			const size_t indicesSizeInBytes, 
			const size_t transformBufferHandle, 
			const size_t instanceCount
			) :
			RenderCommand{ signatureHandle, psoHandle },
			meshHandle{ meshHandle },
			byteOffsetToIndexData{ byteOffsetToIndexData },
			indicesSizeInBytes{ indicesSizeInBytes },
			transformBufferHandle{ transformBufferHandle },
			instanceCount{ instanceCount }		
		{
		}

		void RenderMeshCommand::ExecuteOperationOnResourceReferences
		(
			ResourceRegistryUsingReferences *registry,
			void(ResourceRegistryUsingReferences:: *operation)(size_t)
		)
		{			
			(registry->*operation)(meshHandle);

			if(transformBufferHandle > 0)
			{
				(registry->*operation)(transformBufferHandle);				
			}
			
		}

		void RenderMeshCommand::Record
		(
			RHA::DX12::CmdList *list, 
			ResourceRegistryReadOnly &registry
		)
		{	
			views.vertexView.BufferLocation = registry.GetResourceGPUVirtualAddress(meshHandle);
			views.vertexView.SizeInBytes = byteOffsetToIndexData;
			views.vertexView.StrideInBytes = vertexStride;

			views.indexView.BufferLocation = views.vertexView.BufferLocation + byteOffsetToIndexData;
			views.indexView.Format = DXGI_FORMAT_R32_UINT;
			views.indexView.SizeInBytes = indicesSizeInBytes;


			auto g{ list->AsGraphicsList() };

			g->IASetVertexBuffers(0, 1, &views.vertexView);
			g->IASetIndexBuffer(&views.indexView);
			g->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if(transformBufferHandle > 0)
			{
				g->SetGraphicsRootConstantBufferView(1, registry.GetResourceGPUVirtualAddress(transformBufferHandle));				
			}
			
			g->DrawIndexedInstanced(indicesSizeInBytes / sizeof(unsigned), instanceCount, 0, 0, 0);
			
		}

		
	}

	
}