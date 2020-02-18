#pragma once
#include <ThirdParty/glm/mat4x4.hpp>


namespace Renderer
{
	struct GlobalBufferData
	{
		glm::mat4 view;
		
		glm::mat4 projection;
		
		float time;

		GlobalBufferData() :			
			time{ 0 }
		{			
		}
						
	};
	
	
}
