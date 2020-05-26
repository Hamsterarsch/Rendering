#pragma once
#include <Windows.h>
#include <future>
#include "RendererExportHelper.hpp"
#include "Shared/PtrTypes.hpp"
#include "Resources/MaintainsRenderResources.hpp"

#include "Resources/ResourceHandle.hpp"
#include "Commands/CommandFactory.hpp"



namespace Renderer
{
	class SerializationHook;


	class RENDERER_DLLSPEC Renderer : public virtual MaintainsRenderResources
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Renderer)
		

		public: virtual bool IsBusy() const = 0;
		
		public: virtual void DispatchFrame() = 0;

		public: virtual void RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices, size_t transformBufferHandle = 0, size_t instanceCount = 1) = 0;

		public: virtual void SetCamera(float x, float y, float z, float pitch, float yaw, float roll) = 0;
				
		
		public: virtual void CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
					
		public: virtual void CompilePixelShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
					
		public: virtual void CompileComputeShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
					
		public: virtual void SerializeRootSignature(unsigned cbvAmount, unsigned srvAmount, unsigned uavAmount, unsigned samplerAmount, SerializationHook *serializer) = 0;


		public: virtual ResourceHandle::t_hash MakeWindowsWindowSurface(HWND windowHandle) = 0;

		public: virtual UniquePtr<::Renderer::Commands::CommandFactory> MakeCommandFactory() = 0;

		public: virtual void SubmitCommand(UniquePtr<::Renderer::Commands::Command> &&command) = 0;
		
		public: virtual void DestroyUnreferencedResources() = 0;

		public: virtual void DestroyExecutedCommands() = 0;

		public: virtual void WaitForCommands() = 0;
		
	};


	UniquePtr<Renderer> RENDERER_DLLSPEC MakeRenderer(HWND outputWindow);
	
	
}
