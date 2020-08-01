#pragma once
#include "RendererExportHelper.hpp"
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/EnumFlagOperations.hpp"


namespace Renderer::Commands
{
	enum class RENDERER_DLLSPEC CommandContextEventFlags : unsigned
	{
		Nothing = 0,
		AllBindingsInvalidated = 1 << 0,
		GraphicsBindingsInvalidated = 1 << 1,
		ComputeBindingsInvalidated = 1 << 2,
		InitialContextCommandExecution = 1 << 3
	};
	GENERATE_BINARY_OPERATORS_FOR_ENUM_FLAGS(CommandContextEventFlags)
	
	class RENDERER_DLLSPEC CommandProcessor
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CommandProcessor)


		public: virtual void NotifyCommandContextAbout(CommandContextEventFlags event) = 0;

		public: virtual bool DoesContextEventMatchAll(CommandContextEventFlags eventFlags) const = 0;

		public: virtual bool DoesContextEventMatchAny(CommandContextEventFlags eventFlags) const = 0;

							   		
	};
	
	
}
