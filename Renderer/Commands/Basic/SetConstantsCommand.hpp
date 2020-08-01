#pragma once
#include "Commands/DX12Command.hpp"
#include "Shared/PtrTypes.hpp"
#include <array>


namespace Renderer::DX12::Commands
{	
	class SetConstantsCommand final : public DX12Command
	{
		using t_recordFunction = void(RHA::DX12::CmdList:: *)(unsigned, unsigned, const unsigned &, unsigned);
		
		private: static constexpr unsigned smallDataConstantSize{ 4*4 };
		
		private: std::array<unsigned, smallDataConstantSize> smallConstantData;
		
		private: UniquePtr<unsigned[]> largeConstantData;

		private: unsigned numConstants;

		private: unsigned parameterIndex;
				 
		private: t_recordFunction recordFunction;
		


		public: SetConstantsCommand
		(
			t_recordFunction recordFunction,
			unsigned parameterIndex,
			const unsigned &constantData,
			unsigned numConstants,
			unsigned offsetIntoDstConstants
		);
		
		
		public:	void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) override {}
		
		public: void Execute(DX12CommandProcessor &context) override;
		
	};

	
}

