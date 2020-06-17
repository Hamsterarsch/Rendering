#pragma once
#include "Resources/ResourceHandle.hpp"


namespace App::Core
{
	struct ImageView
	{
		Renderer::ResourceHandle descriptorHandle;
		float uvMinX;
		float uvMinY;
		float uvMaxX;
		float uvMaxY;		
	};

	
}
