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
		const ResourceType type;

		const size_t serial;

		const size_t hash;


		
		public: ResourceHandle(ResourceType type, size_t serial);

			private: static size_t MakeHash(ResourceType type, size_t serial);

		
		public: explicit ResourceHandle(size_t hash);

			private: static ResourceType ExtractTypeFromHash(size_t hash);

			private: static size_t ExtractSerialFromHash(size_t hash);
					 					 		
	};
	
	
}
