#pragma once
#include "RendererExportHelper.hpp"
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/PtrTypes.hpp"
#include "Command.hpp"
#include "Resources/ResourceHandle.hpp"


namespace Renderer::Commands
{		
	class RENDERER_DLLSPEC CommandFactory
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CommandFactory)

		
		public: virtual UniquePtr<Command> PrepareSurfaceForRendering(ResourceHandle::t_hash surface) = 0;

		public: virtual UniquePtr<Command> PresentSurface(ResourceHandle::t_hash surface) = 0;
				/*
		public: virtual UniquePtr<Command> ClearSurface(ResourceHandle::t_hash surface) = 0;

		public: virtual UniquePtr<Command> BindSurface() = 0;

		
		public: virtual UniquePtr<Command> BindPsoToGraphics(ResourceHandle::t_hash pso) = 0;

		public: virtual UniquePtr<Command> BindSignatureToGraphics(ResourceHandle::t_hash signature) = 0;

		
		public: virtual UniquePtr<Command> BindPsoToCompute(ResourceHandle::t_hash pso) = 0;

		public: virtual UniquePtr<Command> BindSignatureToCompute(ResourceHandle::t_hash signature) = 0;
		*/
	};
	
	
}
