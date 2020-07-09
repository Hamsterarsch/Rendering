#pragma once
#include "Math/Interface/Types/Matrix.hpp"


namespace App::Rendering
{
	struct GlobalBufferData
	{
		Math::Matrix view;
		
		Math::Matrix projection;

		Math::Matrix inverseView;

		Math::Matrix inverseProjection;
		
		float time;

		GlobalBufferData() :			
			view{},
			projection{},
			inverseView{},
			inverseProjection{},
			time{ 0 }
		{			
		}
						
	};
	
	
}

