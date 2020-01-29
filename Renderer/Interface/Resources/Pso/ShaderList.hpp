#pragma once


namespace Renderer
{
	struct RENDERER_DLLSPEC Blob
	{
		const void *data{ nullptr };
		size_t sizeInBytes{ 0 };
		
	};
	
	struct RENDERER_DLLSPEC ShaderList
	{
		Blob vs, hs, gs, ds, ps;
			
	};

	
}
