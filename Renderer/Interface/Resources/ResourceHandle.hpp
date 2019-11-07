#pragma once
#include "RendererExportHelper.hpp"


namespace Renderer
{
	enum class RENDERER_DLLSPEC ResourceType : unsigned char
	{
		Mesh,
		Texture,
		PSO
		
	};
	
	struct RENDERER_DLLSPEC ResourceHandle
	{
		ResourceType type;
		size_t serial;
		
	};	
	
}
