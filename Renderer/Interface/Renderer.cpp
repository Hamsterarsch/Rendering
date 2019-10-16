#include "Renderer.hpp"


namespace Renderer
{
	Renderer::Renderer(const unsigned inflightFramesAmount) :
		inflightFramesAmount{ inflightFramesAmount },
		shouldUpdateRendering{ false }
	{
		updaterHandle = std::async( std::launch::async, &Renderer::UpdateRendering, this);
				
		
	}

		int Renderer::UpdateRendering()
		{
			std::lock_guard<std::mutex> lock{ updaterMutex };

			updaterCondition.wait(lock, [&shouldUpdateRendering] { return shouldUpdateRendering; });

			
		
		}
	
}

