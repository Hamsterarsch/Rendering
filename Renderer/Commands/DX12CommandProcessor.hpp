#pragma once
#include "DX12/CmdList.hpp"
#include "Resources/HasQueriableResources.hpp"
#include "Commands/CommandProcessor.hpp"


namespace Renderer::DX12::Commands
{
	enum class CommandContextEvents : short
	{
		Nothing,
		CommandListChanged,
		GraphicsSignatureChanged,
		ComputeSignatureChanged
	};

	
	class DX12CommandProcessor : public ::Renderer::Commands::CommandProcessor
	{		
		public: virtual RHA::DX12::CmdList &GetList() = 0;

		public: virtual HasQueriableResources &GetRegistry() = 0;

		public: virtual void SubmitAndWaitForGpuWork() = 0;

		
		public: virtual void NotifyCommandContextAbout(CommandContextEvents event) = 0;

		public: virtual bool ShouldExecuteContextCommandFor(CommandContextEvents reason) const = 0;
		
	};
	
}
