#pragma once
#include <d3d12.h>
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Descriptor/DescriptorAllocator.hpp"


namespace RHA::DX12{ class WindowSurface; }


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

			public: virtual RHA::DX12::WindowSurface *GetSurface(ResourceHandle::t_hash handle) = 0;

			public: virtual DescriptorAllocator &GetDescriptorAllocator(ResourceHandle::t_hash handle) = 0;
			
			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGpuAddress(ResourceHandle::t_hash handle) = 0;
													
		};

		
	}

	
}