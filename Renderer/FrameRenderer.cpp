#include "FrameRenderer.hpp"


namespace Renderer
{
	namespace DX12
	{
		FrameRenderer::FrameRenderer
		(
			RHA::DX12::DeviceResources* resources, RHA::DX12::Queue* queue,
			unsigned frameWidth, unsigned frameHeight
		) :
			resources{ resources },
			queue{ queue }		
		{
			
		}

		
	}
	
	
}