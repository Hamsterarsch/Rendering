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

			//commands may contain data that needs to be persistent along a frame
			private: struct Views
			{
				D3D12_VERTEX_BUFFER_VIEW vertexView;
				D3D12_INDEX_BUFFER_VIEW indexView;
			} views;
			
			static constexpr size_t vertexStride = sizeof(float) * 3;
			
			//private: std::vector<size_t> bufferHandles;
			//private: std::vector<size_t> textureHandles;
			//private: size_t psoHandle;

			public: RenderMeshCommand(size_t meshHandle, size_t byteOffsetToIndexData, size_t indicesSizeInBytes);
			
			public: virtual size_t GetPsoHandle() const override;
			
			public: virtual size_t GetSignatureHandle() const override;
			
			public: virtual void ExecuteOperationOnResourceReferences(ResourceRegistry* registry, void( ResourceRegistry::* operation)(size_t)) override;

			public: virtual void Record(RHA::DX12::CmdList* list, ResourceRegistry& registry) override;
					
			
		};
		
		
	}
	
	
}
