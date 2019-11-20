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

			constexpr size_t vertexStride = sizeof(float) * 3;
			
			//private: std::vector<size_t> bufferHandles;
			//private: std::vector<size_t> textureHandles;
			//private: size_t psoHandle;

			public: RenderMeshCommand(size_t meshHandle, size_t byteOffsetToIndexData, size_t indicesSizeInBytes);
			
			public:	virtual void RegisterResourceReferences(ResourceRegistry& toRegistry) const override;

			public: virtual void Record(RHA::DX12::CmdList* list, ResourceRegistry& registry, UniquePtr<void> &persistentData) const override;
			
			
		};
		
		
	}
	
	
}
