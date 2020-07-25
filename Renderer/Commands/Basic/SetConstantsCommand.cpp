#include "Commands/Basic/SetConstantsCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetConstantsCommand::SetConstantsCommand
	(
		t_recordFunction recordFunction,
		unsigned parameterIndex,
		const unsigned &constantData,
		unsigned numConstants,
		unsigned offsetIntoConstants
	)	:
		smallConstantData{},
		numConstants{ numConstants },
		parameterIndex{ parameterIndex },
		recordFunction{ recordFunction }
	{
		static_assert(sizeof(unsigned) == 4);
		
		if(numConstants > smallDataConstantSize)
		{
			largeConstantData = MakeUnique<unsigned[]>(numConstants);
			std::memcpy(largeConstantData.get(), &constantData+offsetIntoConstants, numConstants * sizeof(unsigned));			
		}
		else
		{
			std::memcpy(smallConstantData.data(), &constantData+offsetIntoConstants, numConstants * sizeof(unsigned));			
		}
				
	}


	
	void SetConstantsCommand::Execute(DX12CommandProcessor &context)
	{
		if(largeConstantData)
		{
			(context.GetList().*recordFunction)(parameterIndex, numConstants, *largeConstantData.get(), 0);
		}
		else
		{
			(context.GetList().*recordFunction)(parameterIndex, numConstants, *smallConstantData.data(), 0);
		}
		
	}

	
}
