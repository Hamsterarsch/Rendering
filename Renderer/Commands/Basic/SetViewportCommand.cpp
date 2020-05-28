#include "Commands/Basic/SetViewportCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetViewportCommand::SetViewportCommand
	(
		const float topLeftX,
		const float topLeftY,
		const float width,
		const float height,
		const float minDepth,
		const float maxDepth
	)	:
		viewport{ topLeftX, topLeftY, width, height, minDepth, maxDepth }
	{
	}
	
	void SetViewportCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetViewports(&viewport, 1);
		
	}

	
}
