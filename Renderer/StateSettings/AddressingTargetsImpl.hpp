#pragma once
#include "StateSettings/AddressingTargets.hpp"


namespace Renderer::DX12
{
	class AddressingTargetsImpl : public AddressingTargets
	{
		public: const void *AddressingModeWrap() const override;
		
		public: const void *AddressingModeClamp() const override;
		
		public: const void *AddressingModeMirror() const override;


		public: static const AddressingTargetsImpl &Get();
		
	};
		
	
}
