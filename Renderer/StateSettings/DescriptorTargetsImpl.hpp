#pragma once
#include "StateSettings/TargetHelpers.hpp"
#include "StateSettings/DescriptorTargets.hpp"
#include <d3d12.h>


namespace Renderer::DX12
{
	class DescriptorTargetsImpl final : public DescriptorTargets
	{		
		public: const void *ConstantBuffer() const override;
		
		public: const void *ShaderResource() const override;
		
		public: const void *UnorderedAccess() const override;
		

		public: static const DescriptorTargetsImpl &Get();
		
	};

	template<> struct FormatMapping<DescriptorTargets>{ using mapping = DescriptorTargetsImpl; using t_dataDeref = D3D12_DESCRIPTOR_RANGE_TYPE; };
	
}
