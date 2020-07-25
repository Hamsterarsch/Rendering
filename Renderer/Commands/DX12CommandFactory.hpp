#pragma once
#include "Commands/CommandFactory.hpp"


namespace Renderer::DX12{ class ResourceRegistry; }


namespace Renderer::DX12::Commands
{
	class DX12CommandFactory final : public Renderer::Commands::CommandFactory
	{
		private: ResourceRegistry *registry;

		

		public: explicit DX12CommandFactory(ResourceRegistry &registry);

		
		public: UniquePtr<Renderer::Commands::Command> PrepareSurfaceForRendering(ResourceHandle::t_hash surface) override;
		
		public: UniquePtr<Renderer::Commands::Command> PresentSurface(ResourceHandle::t_hash surface) override;

		
		public: UniquePtr<Renderer::Commands::Command> SetSignatureGraphics(ResourceHandle::t_hash signature) override;

		public: UniquePtr<Renderer::Commands::Command> SetSignatureCompute(ResourceHandle::t_hash signature) override;
		
		public: UniquePtr<Renderer::Commands::Command> SetPipelineState(ResourceHandle::t_hash pipeline) override;

		
		public: UniquePtr<Renderer::Commands::Command> SetDescriptorBlockViewsAsGraphicsTable(ResourceHandle::t_hash descriptorBlock, unsigned parameterIndex) override;

		public: UniquePtr<Renderer::Commands::Command> SetDescriptorBlockViewsAsComputeTable(ResourceHandle::t_hash descriptorBlock, unsigned parameterIndex) override;


		public: UniquePtr<Renderer::Commands::Command> BindDepthTargetOnly(ResourceHandle::t_hash depthTextureDescriptor) override;
		
		public: UniquePtr<Renderer::Commands::Command> BindRenderTargets(ResourceHandle::t_hash windowSurface, ResourceHandle::t_hash depthTextureDescriptor) override;

		public: UniquePtr<Renderer::Commands::Command> ClearDepthTexture(ResourceHandle::t_hash depthTextureDescriptor) override;
		
		
		public: UniquePtr<Renderer::Commands::Command> SetIndexBuffer
		(
			ResourceHandle::t_hash indexBuffer,
			size_t byteOffsetToIndices,
			size_t numIndices,
			size_t indexSizeInBytes,
			Format indexFormat
		) override;

		public: UniquePtr<Renderer::Commands::Command> SetVertexBuffer
		(
			ResourceHandle::t_hash vertexBuffer,
			size_t byteOffsetToVertices,
			size_t numVertices,
			size_t vertexStrideInBytes
		) override;

		UniquePtr<Renderer::Commands::Command> SetGraphicConstants
		(
			unsigned parameterIndex, 
			const unsigned &constantData,
			unsigned numConstants,
			unsigned offsetIntoConstants
		) override;

		UniquePtr<Renderer::Commands::Command> SetComputeConstants
		(
			unsigned parameterIndex, 
			const unsigned &constantData,
			unsigned numConstants,
			unsigned offsetIntoDstInConstants
		) override;
		
		
		public: UniquePtr<Renderer::Commands::Command> SetScissorRect(float topLeftX, float topLeftY, float width, float height) override;
				
		public: UniquePtr<Renderer::Commands::Command> SetViewport
		(
			float topLeftX, 
			float topLeftY, 
			float width,
			float height,
			float minDepth,
			float maxDepth
		) override;

						
		UniquePtr<::Renderer::Commands::Command> DrawIndexedInstanced
		(
			size_t instanceCount,
			size_t indexCountPerInstance,
			size_t offsetOntoIndexViewStart,
			size_t offsetOntoIndexValue
		) override;

		public: UniquePtr<Renderer::Commands::Command> Dispatch(unsigned groupSizeX, unsigned groupSizeY, unsigned groupSizeZ) override;
		
		public: UniquePtr<Renderer::Commands::Command> IncreaseCounter(CounterFactory::CounterID id, size_t valueToIncreaseBy) override;

	};
	
	
}
