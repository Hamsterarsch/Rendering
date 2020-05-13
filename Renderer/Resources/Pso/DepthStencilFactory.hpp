#pragma once
#include "Shared/InterfaceHelpers.hpp"


struct D3D12_DEPTH_STENCIL_DESC;


namespace Renderer::DX12
{
	class DepthStencilFactory
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(DepthStencilFactory)


		public: virtual D3D12_DEPTH_STENCIL_DESC MakeDepthStencilDesc() const = 0;
		
	};
	
	
}
