#include "RendererFacade.hpp"
#include "ForwardRenderer.hpp"


namespace Renderer
{

	UniquePtr<RendererFacade> MakeRenderer(HWND outputWindow)
	{
		return std::make_unique<DX12::ForwardRenderer>(outputWindow);
		
	}
	
	
}

