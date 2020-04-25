#pragma once
#include <ThirdParty/glm/mat4x4.hpp>
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
