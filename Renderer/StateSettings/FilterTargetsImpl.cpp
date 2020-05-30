#include "StateSettings/FilterTargetsImpl.hpp"
#include <d3d12.h>


namespace Renderer::DX12
{
	const void *FilterTargetsImpl::FilterMinMagMipLinear() const
	{
		static constexpr auto filter{ D3D12_FILTER_MIN_MAG_MIP_LINEAR };
		return &filter;
		
	}


	
	const FilterTargetsImpl &FilterTargetsImpl::Get()
	{
		static const FilterTargetsImpl instance{};
		return instance;
		
	}

	
}
