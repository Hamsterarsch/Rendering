#pragma once
#include "RendererFacade.hpp"
#include "Asset.hpp"


namespace App::Assets
{
	class RendererAsset : public assetSystem::Asset
	{
		public: virtual void UploadToRenderer(Renderer::RendererFacade &renderer) = 0;
						
	};
	
	
}
