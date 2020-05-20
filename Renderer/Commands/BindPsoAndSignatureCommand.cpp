#include "Commands/BindPsoAndSignatureCommand.hpp"
#include "Resources/UsesReferences.hpp"


namespace Renderer::DX12::Commands
{
	BindPsoAndSignatureCommand::BindPsoAndSignatureCommand() :
		signature{ 0 },
		pso{ 0 }
	{
	}


	
	BindPsoAndSignatureCommand::BindPsoAndSignatureCommand(const size_t signatureHandle, const size_t psoHandle) :
		signature{ signatureHandle },
		pso{ psoHandle }
	{				
	}


	
	void BindPsoAndSignatureCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences:: *operation)(size_t))
	{
		if(PsoIsValid())
		{
			(registry.*operation)(pso);
		}

		if(SignatureIsValid())
		{
			(registry.*operation)(signature);
		}
		
	}

		bool BindPsoAndSignatureCommand::PsoIsValid() const
		{
			return pso > 0;
		
		}
			
		bool BindPsoAndSignatureCommand::SignatureIsValid() const
		{
			return signature > 0;
		
		}

	

	void BindPsoAndSignatureCommand::Execute(CommandProcessor &context)
	{
		RecordPsoBinding(context);
		RecordSignatureBinding( context);
		
	}

		void BindPsoAndSignatureCommand::RecordPsoBinding(CommandProcessor &context) const
		{
			if(PsoIsValid())
			{
				context.GetList().RecordSetPipelineState(context.GetRegistry().GetPso(pso));				
			}
				
		}	

	
}
