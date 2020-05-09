#pragma once
#include "Math/Interface/Types/Matrix.hpp"


namespace Renderer
{
	struct GlobalBufferData
	{
		Math::Matrix view;
		
		Math::Matrix projection;
		
		float time;

		GlobalBufferData() :			
			view{},
			projection{},
			time{ 0 }
		{			
		}
						
	};
	
	
}
