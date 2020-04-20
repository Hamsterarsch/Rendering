#pragma once
#include "ResourceTypes.hpp"
#include "RendererExportHelper.hpp"


namespace Renderer
{	
	struct RENDERER_DLLSPEC ResourceHandle
	{
		using t_hash = size_t;
		
		using t_serial = size_t;
		
		using t_resourceTypes = ResourceTypes;

		
		const t_hash hash;


		
		public: ResourceHandle(t_resourceTypes type, t_serial serial);

			private: static t_hash MakeHash(t_resourceTypes type, t_serial serial);

		public:	explicit ResourceHandle(t_hash hash);

		
		public: t_resourceTypes GetResourceType() const;

			public: static t_resourceTypes GetResourceType(t_hash hash);
			
		public: t_serial GetSerial() const;
					 					 		
	};
	
	
}
