#include "Commands/DX12CommandFactory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Commands/Surface/PrepareSurfaceForRenderingCommand.hpp"
#include "Commands/Surface/PresentSurfaceCommand.hpp"
#include "Commands/Basic/SetSignatureGraphicsCommand.hpp"
#include "Commands/Basic/SetPipelineStateCommand.hpp"
#include "Commands/Basic/SetDescriptorBlockViewsAsGraphicsTableCommand.hpp"
#include "Commands/Basic/SetIndexBufferCommand.hpp"
#include "Commands/Basic/SetVertexBufferCommand.hpp"
#include "Commands/Basic/SetScissorRectCommand.hpp"
#include "Commands/Basic/SetViewportCommand.hpp"
#include "Commands/Basic/DrawIndexedInstancedCommand.hpp"
#include "Commands/IncreaseCounterCommand.hpp"
#include "Basic/SetConstantsToGraphicsCommand.hpp"


namespace Renderer::DX12::Commands
{
	DX12CommandFactory::DX12CommandFactory(ResourceRegistry &registry) :
		registry{ &registry }
	{
	}
		

	UniquePtr<Renderer::Commands::Command> DX12CommandFactory::PrepareSurfaceForRendering(const ResourceHandle::t_hash surface)
	{		
		return MakeUnique<PrepareSurfaceForRenderingCommand>(surface);
		
	}

	UniquePtr<Renderer::Commands::Command> DX12CommandFactory::PresentSurface(const ResourceHandle::t_hash surface)
	{
		return MakeUnique<PresentSurfaceCommand>(surface);
		
	}



	UniquePtr<::Renderer::Commands::Command> DX12CommandFactory::SetSignatureGraphics(const ResourceHandle::t_hash signature)
	{
		return MakeUnique<SetSignatureGraphicsCommand>(signature);
		
	}


	
	UniquePtr<::Renderer::Commands::Command> DX12CommandFactory::SetPipelineState(const ResourceHandle::t_hash pipeline)
	{
		return MakeUnique<SetPipelineStateCommand>(pipeline);
		
	}


	
	UniquePtr<::Renderer::Commands::Command> DX12CommandFactory::SetDescriptorBlockViewsAsGraphicsTable(const ResourceHandle::t_hash descriptorBlock, const unsigned parameterIndex)
	{
		return MakeUnique<SetDescriptorBlockViewsAsGraphicsTableCommand>(descriptorBlock, parameterIndex);
		
	}


	
	UniquePtr<::Renderer::Commands::Command> DX12CommandFactory::SetIndexBuffer
	(
		const ResourceHandle::t_hash indexBuffer,
		const size_t byteOffsetToIndices, 
		const size_t numIndices,
		const size_t indexSizeInBytes,
		const Format indexFormat
	)
	{
		return MakeUnique<SetIndexBufferCommand>(indexBuffer, byteOffsetToIndices, numIndices, indexSizeInBytes, indexFormat, *registry);
		
	}


	
	UniquePtr<::Renderer::Commands::Command> DX12CommandFactory::SetVertexBuffer
	(
		const ResourceHandle::t_hash vertexBuffer, 
		const size_t byteOffsetToVertices, 
		const size_t numVertices, 
		const size_t vertexStrideInBytes
	)
	{
		return MakeUnique<SetVertexBufferCommand>(vertexBuffer, byteOffsetToVertices, numVertices, vertexStrideInBytes, *registry);
		
	}

	UniquePtr<Renderer::Commands::Command> DX12CommandFactory::SetGraphicConstants
	(
		const unsigned parameterIndex,
		const unsigned &constantData,
		const unsigned numConstants,
		const unsigned offsetIntoConstants
	)
	{
		return MakeUnique<SetConstantsToGraphicsCommand>(parameterIndex, constantData, numConstants, offsetIntoConstants);
		
	}



	UniquePtr<::Renderer::Commands::Command> DX12CommandFactory::SetScissorRect
	(
		const float topLeftX,
		const float topLeftY,
		const float width,
		const float height
	)
	{
		return MakeUnique<SetScissorRectCommand>(topLeftX, topLeftY, width, height);
		
	}


	
	UniquePtr<::Renderer::Commands::Command> DX12CommandFactory::SetViewport
	(
		const float topLeftX,
		const float topLeftY,
		const float width,
		const float height,
		const float minDepth,
		const float maxDepth
	)
	{
		return MakeUnique<SetViewportCommand>(topLeftX, topLeftY, width, height, minDepth, maxDepth);
		
	}


	
	UniquePtr<::Renderer::Commands::Command> DX12CommandFactory::DrawIndexedInstanced
	(
		const size_t instanceCount, 
		const size_t indexCountPerInstance, 
		const size_t offsetOntoIndexViewStart, 
		const size_t offsetOntoIndexValue
	)
	{
		return MakeUnique<DrawIndexedInstancedCommand>(instanceCount, indexCountPerInstance, offsetOntoIndexViewStart, offsetOntoIndexValue);
		
	}


	
	UniquePtr<Renderer::Commands::Command> DX12CommandFactory::IncreaseCounter
	(
		const CounterFactory::CounterID id,
		const size_t valueToIncreaseBy
	)
	{
		return MakeUnique<IncreaseCounterCommand>(id, valueToIncreaseBy);
			
	}

	
}
