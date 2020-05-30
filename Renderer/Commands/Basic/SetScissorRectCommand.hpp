#pragma once
#include "Commands/DX12Command.hpp"


namespace Renderer::DX12::Commands
{
	class SetScissorRectCommand final : public DX12Command
	{
		D3D12_RECT rect;



		public: SetScissorRectCommand(float topLeftX, float topLeftY, float width, float height);
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override {}
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};
	
	
}
