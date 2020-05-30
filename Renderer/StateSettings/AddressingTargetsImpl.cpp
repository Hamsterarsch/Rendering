#include "StateSettings/AddressingTargetsImpl.hpp"
#include <d3d12.h>


namespace Renderer::DX12
{
	const void *AddressingTargetsImpl::AddressingModeWrap() const
	{
		static constexpr auto mode{ D3D12_TEXTURE_ADDRESS_MODE_WRAP };
		return &mode;
		
	}


	
	const void *AddressingTargetsImpl::AddressingModeClamp() const
	{
		static constexpr auto mode{ D3D12_TEXTURE_ADDRESS_MODE_CLAMP };
		return &mode;
		
	}


	
	const void *AddressingTargetsImpl::AddressingModeMirror() const
	{
		static constexpr auto mode{ D3D12_TEXTURE_ADDRESS_MODE_MIRROR };
		return &mode;
		
	}


	
	const AddressingTargetsImpl& AddressingTargetsImpl::Get()
	{
		static const AddressingTargetsImpl instance{};
		return instance;
		
	}

	
}
