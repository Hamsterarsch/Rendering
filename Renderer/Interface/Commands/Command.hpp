#pragma once
#include "Commands/CommandProcessor.hpp"
#include "ContainsReferences.hpp"


namespace Renderer::Commands
{
	class RENDERER_DLLSPEC Command : public ContainsReferences
	{			
		public: virtual void Execute(CommandProcessor &context) = 0;

		
	};

	
}
