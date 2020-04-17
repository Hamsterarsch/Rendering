#pragma once
#include <d3d12.h>
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/ResourceHandle.hpp"


namespace Renderer
{
	struct ResourceHandle;
	
	namespace DX12
	{
		class ResourceRegistryReadOnly
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(ResourceRegistryReadOnly)

			
			public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) = 0;

			public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) = 0; 

			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGPUVirtualAddress(ResourceHandle::t_hash handle) = 0;
			
		};

		
	}

	
}