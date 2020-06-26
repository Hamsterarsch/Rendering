#include "StateSettings/DescriptorTargetsImpl.hpp"


namespace Renderer::DX12
{
	const void *DescriptorTargetsImpl::ConstantBuffer() const
	{
		static constexpr auto type{ D3D12_DESCRIPTOR_RANGE_TYPE_CBV };
		return &type;
		
	}


	
	const void *DescriptorTargetsImpl::ShaderResource() const
	{
		static constexpr auto type{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV };
		return &type;
		
	}


	
	const void *DescriptorTargetsImpl::UnorderedAccess() const
	{
		static constexpr auto type{ D3D12_DESCRIPTOR_RANGE_TYPE_UAV };
		return &type;
		
	}


	
	const DescriptorTargetsImpl &DescriptorTargetsImpl::Get()
	{
		static const DescriptorTargetsImpl instance{};
		return instance;
		
	}

	
}
