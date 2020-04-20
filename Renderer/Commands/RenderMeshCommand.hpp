#pragma once
#include "RenderCommand.hpp"


namespace Renderer
{
	namespace DX12
	{
		class RenderMeshCommand : public RenderCommand
		{			
			private: size_t meshHandle;
			
			private: size_t byteOffsetToIndexData;
			
			private: size_t indicesSizeInBytes;

			private: size_t transformBufferHandle;

			private: size_t instanceCount;
			
			//commands may contain data that needs to be persistent along a frame
			private: struct Views
			{
				D3D12_VERTEX_BUFFER_VIEW vertexView;
				D3D12_INDEX_BUFFER_VIEW indexView;
			} views;
			
			static constexpr size_t vertexStride = sizeof(float) * 3;
			
			//private: std::vector<size_t> bufferHandles;
			//
			//private: std::vector<size_t> textureHandles;
			//
			//private: size_t psoHandle;


			
			public: RenderMeshCommand(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t byteOffsetToIndexData, size_t indicesSizeInBytes, size_t transformBufferHandle, size_t instanceCount);
						
			public: virtual void ExecuteOperationOnResourceReferences(ResourceRegistryUsingReferences *registry, void(ResourceRegistryUsingReferences:: *operation)(size_t)) override;

			public: virtual void Record(RHA::DX12::CmdList *list, ResourceRegistryReadOnly &registry) override;
						
		};
		
		
	}
	
	
}
