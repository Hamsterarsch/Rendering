#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class DispatchCommand final : public DX12Command
	{
		unsigned x, y, z;

		

		public: DispatchCommand(unsigned groupSizeX, unsigned groupSizeY, unsigned groupSizeZ) : x{ groupSizeX }, y{ groupSizeY }, z{ groupSizeZ } {}
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override {}
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};

	
}

