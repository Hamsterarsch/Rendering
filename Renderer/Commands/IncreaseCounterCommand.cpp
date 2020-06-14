#include "Commands/IncreaseCounterCommand.hpp"
#include "CounterFactoryImpl.hpp"


namespace Renderer::DX12::Commands
{
	IncreaseCounterCommand::IncreaseCounterCommand(const CounterFactory::CounterID id, const size_t increaseValue) :
		targetCounter{ id },
		increaseValue{ increaseValue }
	{
	}


	
	void IncreaseCounterCommand::Execute(DX12CommandProcessor &context)
	{
		context.SubmitAndWaitForGpuWork();
		context.GetCounters().IncreaseCounter(targetCounter, increaseValue);
		
	}

	
}
