#pragma once
#include "RendererExportHelper.hpp"
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer::Commands
{
	enum class RENDERER_DLLSPEC CommandContextEvents : short
	{
		Nothing,
		AllBindingsInvalidated,
		GraphicsBindingsInvalidated,
		ComputeBindingsInvalidated
	};
		
	
	class RENDERER_DLLSPEC CommandProcessor
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CommandProcessor)


		public: virtual void NotifyCommandContextAbout(CommandContextEvents event) = 0;

		public: virtual bool DoesContextEventMatch(CommandContextEvents reason) const = 0;

							   		
	};
	
	
}
