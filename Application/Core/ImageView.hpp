#pragma once
#include "Resources/ResourceHandle.hpp"


namespace App::Core
{
	struct ImageView
	{
		Renderer::ResourceHandle::t_hash descriptorHandle;
		float uvMinX;
		float uvMinY;
		float uvMaxX;
		float uvMaxY;


		
		ImageView() : ImageView{ 0 } {}
		
		ImageView(Renderer::ResourceHandle::t_hash descriptorHandle) : ImageView{ descriptorHandle, 0, 0, 1, 1 } {}
		
		ImageView(Renderer::ResourceHandle::t_hash descriptorHandle, float uvMinX, float uvMinY, float uvMaxX, float uvMaxY)
			:
			descriptorHandle{ descriptorHandle },
			uvMinX{ uvMinX },
			uvMinY{ uvMinY },
			uvMaxX{ uvMaxX },
			uvMaxY{ uvMaxY }
		{}
		
	};

	
}
