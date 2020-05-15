#pragma once
#include "Math/Interface/Types/Matrix.hpp"


namespace Renderer
{
	struct GlobalBufferData
	{
		Math::Matrix view;
		
		Math::Matrix projection;

		Math::Matrix inverseViewProjection;
		
		float time;

		GlobalBufferData() :			
			view{},
			projection{},
			inverseViewProjection{},
			time{ 0 }
		{			
		}
						
	};
	
	
}

