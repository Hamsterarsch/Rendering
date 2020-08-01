#pragma once
#include "DX12/CmdList.hpp"
#include "Resources/HasQueriableResources.hpp"
#include "Commands/CommandProcessor.hpp"

namespace Renderer::DX12{ class CounterFactoryImpl; }



namespace Renderer::DX12::Commands
{

	
	class DX12CommandProcessor : public ::Renderer::Commands::CommandProcessor
	{		
		public: virtual RHA::DX12::CmdList &GetList() = 0;

		public: virtual HasQueriableResources &GetRegistry() = 0;

		public: virtual void SubmitAndWaitForGpuWork() = 0;


		public: virtual CounterFactoryImpl &GetCounters() = 0;
		
	};
	
}
