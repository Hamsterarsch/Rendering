#include <d3d12.h>
#include "DX12/CmdList.hpp"
#include "RenderMeshCommand.hpp"


namespace Renderer
{
	namespace DX12
	{
		struct Views
		{
			D3D12_VERTEX_BUFFER_VIEW vertexView;
			D3D12_INDEX_BUFFER_VIEW indexView;
		};

		RenderMeshCommand::RenderMeshCommand(const size_t meshHandle, const size_t byteOffsetToIndexData, const size_t indicesSizeInBytes) :
			meshHandle{ meshHandle },
			byteOffsetToIndexData{ byteOffsetToIndexData },
			indicesSizeInBytes{ indicesSizeInBytes }			
		{
		}

		void RenderMeshCommand::ExecuteOperationOnResourceReferences
		(
			ResourceRegistry *registry,
			void( ResourceRegistry:: *operation)(size_t)
		)
		{
			(registry->*operation)(meshHandle);
			
		}

		void RenderMeshCommand::Record
		(
			RHA::DX12::CmdList* list, 
			ResourceRegistry& registry,
			UniquePtr<void>& persistentData
		) const
		{
			auto mesh{ registry.GetResource(meshHandle) };
						
			persistentData = std::make_unique<Views>();
			auto data{ reinterpret_cast<Views *>(persistentData.get()) };
			
			data->vertexView.BufferLocation = mesh->GetGPUVirtualAddress();
			data->vertexView.SizeInBytes = byteOffsetToIndexData;
			data->vertexView.StrideInBytes = vertexStride;

			data->indexView.BufferLocation = data->vertexView.BufferLocation + byteOffsetToIndexData;
			data->indexView.Format = DXGI_FORMAT_R32_UINT;
			data->indexView.SizeInBytes = indicesSizeInBytes;


			auto g{ list->AsGraphicsList() };

			g->IASetVertexBuffers(0, 1, &data->vertexView);
			g->IASetIndexBuffer(&data->indexView);
			g->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			g->DrawIndexedInstanced(indicesSizeInBytes / sizeof(unsigned), 1, 0, 0, 0);
			
		}

		
	}

	
}