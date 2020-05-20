#pragma once
#include "Commands/CommandProcessor.hpp"
#include "Resources/UsesReferences.hpp"


namespace Renderer::DX12::Commands
{
	class Command
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Command)


		public: virtual void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) = 0;
			
		public: virtual void Execute(CommandProcessor &context) = 0;

	};


}
