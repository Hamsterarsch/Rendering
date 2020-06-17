#pragma once
#include "Commands/DX12Command.hpp"
#include "Shared/PtrTypes.hpp"
#include <array>


namespace Renderer::DX12::Commands
{
	class SetConstantsToGraphicsCommand final : public DX12Command
	{
		private: static constexpr unsigned smallDataConstantSize{ 4*4 };
		
		private: std::array<unsigned, smallDataConstantSize> smallConstantData;
		
		private: UniquePtr<unsigned[]> largeConstantData;

		private: unsigned numConstants;

		private: unsigned parameterIndex;
		


		public: SetConstantsToGraphicsCommand(unsigned parameterIndex, const unsigned &constantData, unsigned numConstants, unsigned offsetIntoConstants);
		
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override {}
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};

	
}

