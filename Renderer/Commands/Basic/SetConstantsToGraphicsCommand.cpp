#include "Commands/Basic/SetConstantsToGraphicsCommand.hpp"


namespace Renderer::DX12::Commands
{
	SetConstantsToGraphicsCommand::SetConstantsToGraphicsCommand
	(
		const unsigned parameterIndex,
		const unsigned &constantData,
		const unsigned numConstants,
		const unsigned offsetIntoConstants
	)	:
		smallConstantData{},
		numConstants{ numConstants },
		parameterIndex{ parameterIndex }
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

	void SetConstantsToGraphicsCommand::Execute(DX12CommandProcessor &context)
	{
		if(largeConstantData)
		{
			context.GetList().RecordSetGraphicsConstants(parameterIndex, numConstants, *largeConstantData.get(), 0);
		}
		else
		{
			context.GetList().RecordSetGraphicsConstants(parameterIndex, numConstants, *smallConstantData.data(), 0);
		}
		
	}

	
}
