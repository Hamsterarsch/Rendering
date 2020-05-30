#include "Commands/Basic/SetScissorRectCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetScissorRectCommand::SetScissorRectCommand
	(
		const float topLeftX,
		const float topLeftY,
		const float width,
		const float height
	)			
	{
		rect.left = topLeftX;
		rect.right = topLeftX + width;

		rect.top = topLeftY;
		rect.bottom = topLeftY + height;
		
	}

	void SetScissorRectCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordSetScissorRects(&rect, 1);
		
	}

	
}
