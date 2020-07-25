#pragma once
#include "RendererExportHelper.hpp"
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/PtrTypes.hpp"
#include "Command.hpp"
#include "Resources/ResourceHandle.hpp"
#include "StateSettings/FormatTargets.hpp"
#include "CounterFactory.hpp"


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
				*/

		public: virtual UniquePtr<Command> SetSignatureGraphics(ResourceHandle::t_hash signature) = 0;

		public: virtual UniquePtr<Command> SetSignatureCompute(ResourceHandle::t_hash signature) = 0;

		public: virtual UniquePtr<Command> SetPipelineState(ResourceHandle::t_hash pipeline) = 0;

		
		public: virtual UniquePtr<Command> SetDescriptorBlockViewsAsGraphicsTable(ResourceHandle::t_hash descriptorBlock, unsigned parameterIndex) = 0;

		public: virtual UniquePtr<Command> SetDescriptorBlockViewsAsComputeTable(ResourceHandle::t_hash descriptorBlock, unsigned parameterIndex) = 0;

		
		public: virtual UniquePtr<Command>  BindDepthTargetOnly(ResourceHandle::t_hash depthTextureDescriptor) = 0;
		
		public: virtual UniquePtr<Command> BindRenderTargets(ResourceHandle::t_hash windowSurface, ResourceHandle::t_hash depthTextureDescriptor) = 0;

		public: virtual UniquePtr<Command> ClearDepthTexture(ResourceHandle::t_hash depthTextureDescriptor) = 0;
		
		
		public: virtual UniquePtr<Command> SetIndexBuffer
		(
			ResourceHandle::t_hash indexBuffer,
			size_t byteOffsetToIndices,
			size_t numIndices,
			size_t indexSizeInBytes,
			Format indexFormat
		) = 0;

		public: virtual UniquePtr<Command> SetVertexBuffer
		(
			ResourceHandle::t_hash vertexBuffer,
			size_t byteOffsetToVertices,
			size_t numVertices,
			size_t vertexStrideInBytes
		) = 0;

		public: virtual UniquePtr<Command> SetGraphicConstants
		(
			unsigned parameterIndex,
			const unsigned &constantData,
			unsigned numConstants,
			unsigned offsetIntoDstInConstants
		) = 0;

		public: virtual UniquePtr<Command> SetComputeConstants
		(
			unsigned parameterIndex, 
			const unsigned &constantData,
			unsigned numConstants,
			unsigned offsetIntoDstInConstants
		) = 0;
		

		public: virtual UniquePtr<Command> SetScissorRect(float topLeftX, float topLeftY, float width, float height) = 0;
				
		public: virtual UniquePtr<Command> SetViewport
		(
			float topLeftX, 
			float topLeftY, 
			float width,
			float height,
			float minDepth,
			float maxDepth
		) = 0;


		public: virtual UniquePtr<Command> DrawIndexedInstanced
		(
			size_t instanceCount,
			size_t indexCountPerInstance,
			size_t offsetOntoIndexViewStart,
			size_t offsetOntoIndexValue
		) = 0;

		public: virtual UniquePtr<Command> Dispatch(unsigned dimensionX, unsigned dimensionY, unsigned dimensionZ) = 0;

		public: virtual UniquePtr<Command> IncreaseCounter(CounterFactory::CounterID id, size_t valueToIncreaseBy) = 0;
		
	};
	
	
}
