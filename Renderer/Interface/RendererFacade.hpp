#pragma once
#include <Windows.h>
#include <future>
#include "RendererExportHelper.hpp"
#include "Shared/PtrTypes.hpp"
#include "Resources/MaintainsRenderResources.hpp"

#include "Resources/ResourceHandle.hpp"
#include "Commands/CommandFactory.hpp"

#include "StateSettings/BlendSettings.hpp"
#include "StateSettings/DepthStencilSettings.hpp"
#include "StateSettings/VertexLayoutSettings.hpp"
#include "StateSettings/RasterizerSettings.hpp"
#include "StateSettings/SamplerSpec.hpp"
#include "ResourceViewFactory.hpp"

namespace Renderer
{
	class SerializationHook;


	class RENDERER_DLLSPEC RendererFacade : public virtual MaintainsRenderResources
	{		
		public: virtual void CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
					
		public: virtual void CompilePixelShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
					
		public: virtual void CompileComputeShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
					
		public: virtual void SerializeRootSignature
		(
			unsigned cbvAmount,
			unsigned srvAmount,
			unsigned uavAmount,
			unsigned samplerAmount, 
			SerializationHook *serializer,
			const SamplerSpec *staticSamplers,
			unsigned numStaticSamplers
		)	= 0;


		
		public: virtual ResourceHandle::t_hash MakeWindowsWindowSurface(HWND windowHandle) = 0;

		
		public: virtual UniquePtr<Commands::CommandFactory> MakeCommandFactory() = 0;

		public: virtual void SubmitCommand(UniquePtr<Commands::Command> &&command) = 0;

		public: virtual void SubmitContextCommand(UniquePtr<Commands::Command> &&command) = 0;
		
		public: virtual void DestroyUnreferencedResources() = 0;

		public: virtual void DestroyExecutedCommands() = 0;

		public: virtual void WaitForCommands() = 0;


		public: virtual BlendSettings &GetBlendSettings() = 0;

		public: virtual DepthStencilSettings &GetDepthStencilSettings() = 0;

		public: virtual RasterizerSettings &GetRasterizerSettings() = 0;

		public: virtual VertexLayoutSettings &GetVertexLayoutSettings() = 0;


		public: virtual ResourceViewFactory &GetViewFactory() = 0;
				
	};


	UniquePtr<RendererFacade> RENDERER_DLLSPEC MakeRenderer(HWND outputWindow);
	
	
}
