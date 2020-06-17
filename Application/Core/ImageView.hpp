#pragma once
#include "Resources/ResourceHandle.hpp"


namespace App::Core
{
	struct ImageView
	{
		Renderer::ResourceHandle handle;
		float uvMinX;
		float uvMinY;
		float uvMaxX;
		float uvMaxy;		
	};

	
}
