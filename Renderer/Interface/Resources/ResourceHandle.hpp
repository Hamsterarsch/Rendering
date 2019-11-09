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
		using t_hash = size_t;
		
		using t_serial = size_t;
		
		using t_resourceType = ResourceType;

		
		const t_hash hash;


		
		public: ResourceHandle(t_resourceType type, t_serial serial);

			private: static t_hash MakeHash(t_resourceType type, t_serial serial);
					 
		public: t_resourceType GetResourceType() const;
			
		public: t_serial GetSerial() const;
					 					 		
	};
	
	
}
