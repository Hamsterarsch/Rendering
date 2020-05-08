#pragma once
#include "Resources/MaintainsRenderResources.hpp"


namespace Renderer
{
	class RENDERER_DLLSPEC MaintainsInternalRenderResources : public virtual MaintainsRenderResources
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(MaintainsInternalRenderResources)


		public: virtual size_t MakeUavBuffer(const void *data, size_t sizeInBytes) = 0;

		public: virtual DxPtr<ID3D12Resource> MakeReadbackBuffer(size_t sizeInBytes) = 0;
				
	};

	
}
