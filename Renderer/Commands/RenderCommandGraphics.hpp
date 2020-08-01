#pragma once
#include "Commands/BindPsoAndSignatureCommand.hpp"

#include "DX12/CmdList.hpp"
#include "Resources/HasQueriableResources.hpp"

namespace Renderer::DX12::Commands
{	
	class RenderCommandGraphics : public BindPsoAndSignatureCommand
	{
		public: RenderCommandGraphics(size_t signatureHandle, size_t psoHandle) : BindPsoAndSignatureCommand{ signatureHandle, psoHandle } {}
		
		public:	void RecordSignatureBinding(DX12CommandProcessor &context) const final override 
		{
			if(SignatureIsValid())
			{				
				context.GetList().RecordSetGraphicsSignature(context.GetRegistry().GetSignature(GetSignatureHandle()));
				context.NotifyCommandContextAbout(Renderer::Commands::CommandContextEventFlags::GraphicsBindingsInvalidated);				
			}			
					
		}
						
	};
	
	
}
