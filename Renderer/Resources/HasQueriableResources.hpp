#pragma once
#include <d3d12.h>
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/ResourceHandle.hpp"


namespace Renderer
{
	struct ResourceHandle;
	
	namespace DX12
	{
		class HasQueriableResources
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(HasQueriableResources)

			
			public: virtual ID3D12Resource *GetResource(ResourceHandle::t_hash handle) = 0;
			
			public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) = 0;

			public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) = 0; 

			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGpuAddress(ResourceHandle::t_hash handle) = 0;
			
		};

		
	}

	
}