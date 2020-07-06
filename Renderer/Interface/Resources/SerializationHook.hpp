#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "RendererExportHelper.hpp"

namespace Renderer
{
	class RENDERER_DLLSPEC SerializationHook
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(SerializationHook)
					
		public: virtual void Resize(size_t sizeInBytes) = 0;

		public: virtual const unsigned char *GetData() const = 0;
		
		public: virtual unsigned char *GetData() = 0;
							
	};

	
}
