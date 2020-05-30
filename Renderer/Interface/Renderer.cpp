#include "RendererFacade.hpp"
#include "RendererFacadeImpl.hpp"


namespace Renderer
{

	UniquePtr<RendererFacade> MakeRenderer(HWND outputWindow)
	{
		return std::make_unique<DX12::RendererFacadeImpl>(outputWindow);
		
	}
	
	
}

