#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class SetViewportCommand final : public DX12Command
	{
		D3D12_VIEWPORT viewport;



		public: SetViewportCommand(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override {}
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};

	
}

