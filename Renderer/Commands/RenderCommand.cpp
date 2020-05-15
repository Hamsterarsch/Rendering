#include "Commands/RenderCommand.hpp"
#include "Resources/UsesReferences.hpp"


namespace Renderer::DX12
{
	RenderCommand::RenderCommand() :
		signatureHandle{ 0 },
		psoHandle{ 0 }
	{
	}


	
	RenderCommand::RenderCommand(const size_t signatureHandle, const size_t psoHandle) :
		signatureHandle{ signatureHandle },
		psoHandle{ psoHandle }
	{				
	}
		

		
	void RenderCommand::AddSignatureReference(UsesReferences &to) const
	{
		if(SignatureIsValid())
		{
			to.AddReference(signatureHandle);			
		}
		
	}

		bool RenderCommand::SignatureIsValid() const
		{
			return signatureHandle > 0;
		
		}



	void RenderCommand::AddPsoReference(UsesReferences &to) const
	{
		if(PsoIsValid())
		{
			to.AddReference(psoHandle);			
		}
		
	}
	   	
		bool RenderCommand::PsoIsValid() const
		{
			return psoHandle > 0;
		}

	
}
