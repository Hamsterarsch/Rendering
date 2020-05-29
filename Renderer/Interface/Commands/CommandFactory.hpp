#pragma once
#include "RendererExportHelper.hpp"
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/PtrTypes.hpp"
#include "Command.hpp"
#include "Resources/ResourceHandle.hpp"
#include "StateSettings/FormatTargets.hpp"


namespace Renderer::Commands
{		
	class RENDERER_DLLSPEC CommandFactory
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CommandFactory)

		
		public: virtual UniquePtr<Command> PrepareSurfaceForRendering(ResourceHandle::t_hash surface) = 0;

		public: virtual UniquePtr<Command> PresentSurface(ResourceHandle::t_hash surface) = 0;
				/*
		public: virtual UniquePtr<Command> ClearSurface(ResourceHandle::t_hash surface) = 0;

		public: virtual UniquePtr<Command> BindSurface() = 0;

		
		public: virtual UniquePtr<Command> BindPsoToGraphics(ResourceHandle::t_hash pso) = 0;

		public: virtual UniquePtr<Command> BindSignatureToGraphics(ResourceHandle::t_hash signature) = 0;

		
		public: virtual UniquePtr<Command> BindPsoToCompute(ResourceHandle::t_hash pso) = 0;

		public: virtual UniquePtr<Command> BindSignatureToCompute(ResourceHandle::t_hash signature) = 0;
		*/

		public: virtual UniquePtr<::Renderer::Commands::Command> SetSignatureGraphics(ResourceHandle::t_hash signature) = 0;

		public: virtual UniquePtr<::Renderer::Commands::Command> SetPipelineState(ResourceHandle::t_hash pipeline) = 0;

		
		public: virtual UniquePtr<::Renderer::Commands::Command> SetDescriptorBlockViewsGraphics(ResourceHandle::t_hash descriptorBlock) = 0;

		
		public: virtual UniquePtr<::Renderer::Commands::Command> SetIndexBuffer
		(
			ResourceHandle::t_hash indexBuffer,
			size_t byteOffsetToIndices,
			size_t numIndices,
			size_t indexSizeInBytes,
			t_format_target indexFormat
		) = 0;

		public: virtual UniquePtr<::Renderer::Commands::Command> SetVertexBuffer
		(
			ResourceHandle::t_hash vertexBuffer,
			size_t byteOffsetToVertices,
			size_t numVertices,
			size_t vertexStrideInBytes
		) = 0;


		public: virtual UniquePtr<::Renderer::Commands::Command> SetScissorRect(float topLeftX, float topLeftY, float width, float height) = 0;
				
		public: virtual UniquePtr<::Renderer::Commands::Command> SetViewport
		(
			float topLeftX, 
			float topLeftY, 
			float width,
			float height,
			float minDepth,
			float maxDepth
		) = 0;


		public: virtual UniquePtr<::Renderer::Commands::Command> DrawIndexedInstanced
		(
			size_t instanceCount,
			size_t indexCountPerInstance,
			size_t offsetOntoIndexViewStart,
			size_t offsetOntoIndexValue
		) = 0;
		
	};
	
	
}
