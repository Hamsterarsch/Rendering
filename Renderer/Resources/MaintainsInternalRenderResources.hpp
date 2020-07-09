#pragma once
#include "Resources/MaintainsRenderResources.hpp"


namespace Renderer
{
	class RENDERER_DLLSPEC MaintainsInternalRenderResources : public virtual MaintainsRenderResources
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(MaintainsInternalRenderResources)


		using MaintainsRenderResources::MakeBuffer;
		
		public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes, D3D12_RESOURCE_STATES state) = 0;
				
		public: virtual DxPtr<ID3D12Resource> MakeReadbackBuffer(size_t sizeInBytes) = 0;
				
	};

	
}
