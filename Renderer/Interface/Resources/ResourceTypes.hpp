#pragma once
#include "RendererExportHelper.hpp"


namespace Renderer
{
	enum class RENDERER_DLLSPEC ResourceTypes : unsigned char
	{
		Mesh,
		Texture,
		Pso,
		Buffer,
		Signature,		
		WindowSurface,
		DescriptorAllocator,
		DepthTexture,
		RawDescriptor,
		ReadbackBuffer,
		UploadResource,
		
	};

	
}
