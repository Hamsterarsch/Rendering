#include "FrameRenderer.hpp"
#include <d3d12.h>
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
#include "DX12/DeviceResources.hpp"


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

		FrameRenderer::~FrameRenderer() noexcept = default;
			
				
	}
	
	
}