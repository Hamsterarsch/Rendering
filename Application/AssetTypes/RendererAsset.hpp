#pragma once
#include "RendererFacade.hpp"
#include "Shared/InterfaceHelpers.hpp"


namespace App::Assets
{
	class RendererAsset
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(RendererAsset)


		public: virtual void UploadToRenderer(Renderer::RendererFacade &renderer) = 0;
				
	};
	
	
}
