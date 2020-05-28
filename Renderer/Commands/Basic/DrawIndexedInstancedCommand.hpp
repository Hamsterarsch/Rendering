#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class DrawIndexedInstancedCommand final : public DX12Command
	{
		private: size_t instanceCount;

		private: size_t indexCountPerInstance;

		private: size_t offsetOntoIndexViewStart;

		private: size_t offsetOntoIndexValue;

		
		
		public: DrawIndexedInstancedCommand(size_t instanceCount, size_t indexCountPerInstance, size_t offsetOntoIndexViewStart, size_t offsetOntoIndexValue);
		
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override {}
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};

	
}

