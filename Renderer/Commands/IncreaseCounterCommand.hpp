#pragma once
#include "DX12Command.hpp"
#include "CounterFactory.hpp"


namespace Renderer::DX12::Commands
{
	class IncreaseCounterCommand final : public DX12Command
	{
		private: CounterFactory::CounterID targetCounter;

		private: size_t increaseValue;


		public: IncreaseCounterCommand(CounterFactory::CounterID id, size_t increaseValue);
		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override {}
		
		public: void Execute(DX12CommandProcessor &context) override;
						
	};
	
	
}
