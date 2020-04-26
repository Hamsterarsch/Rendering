#include "Renderer.hpp"
#include "ForwardRenderer.hpp"


namespace Renderer
{

	UniquePtr<Renderer> MakeRenderer(HWND outputWindow)
	{
		return std::make_unique<DX12::ForwardRenderer>(outputWindow);
		
	}
	
	
}

