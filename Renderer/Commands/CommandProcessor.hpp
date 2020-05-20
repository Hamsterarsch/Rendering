#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DX12/CmdList.hpp"
#include "Resources/HasQueriableResources.hpp"


namespace Renderer::DX12::Commands
{
	enum class CommandContextEvents : short
	{
		Nothing,
		CommandListChanged,
		GraphicsSignatureChanged,
		ComputeSignatureChanged
	};

	
	class CommandProcessor
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CommandProcessor)

		
		public: virtual RHA::DX12::CmdList &GetList() = 0;

		public: virtual HasQueriableResources &GetRegistry() = 0;

		public: virtual void SubmitAndWaitForGpuWork() = 0;

		
		public: virtual void NotifyCommandContextAbout(CommandContextEvents event) = 0;

		public: virtual bool ShouldExecuteContextCommandFor(CommandContextEvents reason) const = 0;
		
	};
	
}
