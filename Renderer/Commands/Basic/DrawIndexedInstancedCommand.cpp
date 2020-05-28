#include "Commands/Basic/DrawIndexedInstancedCommand.hpp"


namespace Renderer::DX12::Commands
{
	DrawIndexedInstancedCommand::DrawIndexedInstancedCommand
	(
		const size_t instanceCount,
		const size_t indexCountPerInstance,
		const size_t offsetOntoIndexViewStart,
		const size_t offsetOntoIndexValue
	)	:
		instanceCount{ instanceCount },
		indexCountPerInstance{ indexCountPerInstance },
		offsetOntoIndexViewStart{ offsetOntoIndexViewStart },
		offsetOntoIndexValue{ offsetOntoIndexValue }
	{
	}


	
	void DrawIndexedInstancedCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordDrawIndexedInstanced(instanceCount, indexCountPerInstance, offsetOntoIndexViewStart, offsetOntoIndexValue, 0);
		
	}

	
}
