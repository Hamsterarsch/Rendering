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
						/*
		public: UniquePtr<Renderer::Commands::Command> ClearSurface(ResourceHandle::t_hash surface) override;
		
		public: UniquePtr<Renderer::Commands::Command> BindSurface() override;

		
		public: UniquePtr<Renderer::Commands::Command> BindPsoToGraphics(ResourceHandle::t_hash pso) override;
		
		public: UniquePtr<Renderer::Commands::Command> BindSignatureToGraphics(ResourceHandle::t_hash signature) override;

		
		public: UniquePtr<Renderer::Commands::Command> BindPsoToCompute(ResourceHandle::t_hash pso) override;
		
		public: UniquePtr<Renderer::Commands::Command> BindSignatureToCompute(ResourceHandle::t_hash signature) override;
		*/


		public: UniquePtr<Renderer::Commands::Command> SetSignatureGraphics(ResourceHandle::t_hash signature) override;

		public: UniquePtr<Renderer::Commands::Command> SetPipelineState(ResourceHandle::t_hash pipeline) override;

		
		public: UniquePtr<Renderer::Commands::Command> SetDescriptorBlockViewsAsGraphicsTable(ResourceHandle::t_hash descriptorBlock, unsigned parameterIndex) override;

		
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
		
		public: UniquePtr<Renderer::Commands::Command> IncreaseCounter(CounterFactory::CounterID id, size_t valueToIncreaseBy) override;
		
	};
	
	
}
