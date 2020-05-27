#include "StateSettings/FormatTargetsImpl.hpp"
#include <d3d12.h>


namespace Renderer::DX12
{
	const void *FormatTargetsImpl::R32G32B32_Float() const
	{
		static constexpr auto format{ DXGI_FORMAT_R32G32B32_FLOAT };
		return &format;	
		
	}



	const void *FormatTargetsImpl::R32G32_Float() const
	{
		static constexpr auto format{ DXGI_FORMAT_R32G32_FLOAT };
		return &format;
		
	}


	
	const void *FormatTargetsImpl::R8G8B8A8_Norm() const
	{
		static constexpr auto format{ DXGI_FORMAT_R8G8B8A8_SNORM };
		return &format;
		
	}



	const FormatTargetsImpl &FormatTargetsImpl::Get()
	{
		static const FormatTargetsImpl instance{};
		return instance;
		
	}

	
}
