#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer
{
	class DescriptorTargets
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(DescriptorTargets)

		
		public: virtual const void *ConstantBuffer() const = 0;

		public: virtual const void *ShaderResource() const = 0;

		public: virtual const void *UnorderedAccess() const = 0;
							   		
	};

	using DescriptorTarget = const void *(DescriptorTargets:: *)() const;

	
}