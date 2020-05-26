#pragma once
#include "Commands/RenderCommandGraphics.hpp"
#include "Resources/Descriptor/DescriptorAllocator.hpp"


namespace Renderer::DX12::Commands
{
	struct RenderMeshArguments
	{
		size_t signature;

		size_t pso;

		size_t instanceTransforms;

		size_t instanceCount;

		size_t mesh;

		size_t byteOffsetToIndexData;

		size_t indicesSizeInBytes;

	};

	class RenderMeshCommand : public RenderCommandGraphics
	{			
		private: size_t meshHandle;
		
		private: size_t byteOffsetToIndexData;
		
		private: size_t indicesSizeInBytes;

		private: size_t transformBufferHandle;

		private: size_t instanceCount;
		
		private: size_t gridDataBuffer;

		private: size_t lightBuffer;

		private: size_t relevantLightsList;

		private: size_t offsetsToRelevantLightsList;

		private: DescriptorAllocator *descAlloc;

		private: struct Views
		{
			D3D12_VERTEX_BUFFER_VIEW vertexView;
			D3D12_INDEX_BUFFER_VIEW indexView;
		} views;
		
		static constexpr size_t vertexStride = sizeof(float) * 6;
		

		
		public: RenderMeshCommand(const RenderMeshArguments &arguments);

		public: RenderMeshCommand(size_t signature, size_t pso, const RenderMeshArguments &arguments);

		public: RenderMeshCommand
		(
			size_t signature,
			size_t pso,
			size_t gridDataBuffer,
			size_t lightsBuffer, 
			size_t relevantLightsList,
			size_t offsetToRelevantLightsList,
			const RenderMeshArguments &arguments,
			DescriptorAllocator &filledDescAlloc
		);
					
		public: virtual void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override;

		public: virtual void Execute(DX12CommandProcessor &context) override;

	};
	
	
}
