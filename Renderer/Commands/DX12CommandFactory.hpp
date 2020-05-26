#pragma once
#include "Commands/CommandFactory.hpp"


namespace Renderer::DX12{ class ResourceRegistry; }


namespace Renderer::DX12::Commands
{
	class DX12CommandFactory final : public ::Renderer::Commands::CommandFactory
	{
		private: ResourceRegistry *registry;

		

		public: explicit DX12CommandFactory(ResourceRegistry &registry);
		
		public: UniquePtr<::Renderer::Commands::Command> PrepareSurfaceForRendering(ResourceHandle::t_hash surface) override;
		
		public: UniquePtr<::Renderer::Commands::Command> PresentSurface(ResourceHandle::t_hash surface) override;
						/*
		public: UniquePtr<Renderer::Commands::Command> ClearSurface(ResourceHandle::t_hash surface) override;
		
		public: UniquePtr<Renderer::Commands::Command> BindSurface() override;

		
		public: UniquePtr<Renderer::Commands::Command> BindPsoToGraphics(ResourceHandle::t_hash pso) override;
		
		public: UniquePtr<Renderer::Commands::Command> BindSignatureToGraphics(ResourceHandle::t_hash signature) override;

		
		public: UniquePtr<Renderer::Commands::Command> BindPsoToCompute(ResourceHandle::t_hash pso) override;
		
		public: UniquePtr<Renderer::Commands::Command> BindSignatureToCompute(ResourceHandle::t_hash signature) override;
		*/
	};
	
	
}
