#pragma once
#include "Commands/Command.hpp"
#include "Resources/UsesReferences.hpp"
#include "Commands/DX12CommandProcessor.hpp"


namespace Renderer::DX12::Commands
{
	class DX12Command : public ::Renderer::Commands::Command
	{		
		private: void Execute(::Renderer::Commands::CommandProcessor &context) final override
		{			
			Execute(static_cast<DX12CommandProcessor &>(context));
			
		}
				 
		public: virtual void Execute(DX12CommandProcessor &context) = 0;

	};


}
