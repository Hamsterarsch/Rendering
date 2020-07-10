#include "Renderer/Commands/Basic/DispatchCommand.hpp"


namespace Renderer::DX12::Commands
{
	void DispatchCommand::Execute(DX12CommandProcessor &context)
	{
		context.GetList().RecordDispatch(x, y, z);
		
	}

	
}
