#pragma once
#include "Interface/RendererFacade.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Resources/Pso/PsoFactory.hpp"
#include "Resources/RootSignature/RootSignatureFactory.hpp"
#include "DX12/ShaderFactory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "Commands/CommandProcessorImpl.hpp"
#include "Commands/RenderMeshCommand.hpp"
#include "Commands/CommandFactory.hpp"
#include "StateSettings/BlendSettingsImpl.hpp"
#include "StateSettings/DepthStencilSettingsImpl.hpp"
#include "StateSettings/VertexLayoutSettingsImpl.hpp"
#include "StateSettings/RasterizerSettingsImpl.hpp"
#include "Resources/Descriptor/ResourceViewFactoryImpl.hpp"


namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
		class Queue;
		class CmdAllocator;
		class CmdList;
		class WindowSurface;
		class Fence;
		class UploadHeap;
		class DepthSurface;
	}
}


namespace Renderer::DX12::Commands{ class InitVolumeTileGridCommand; }

namespace Renderer::DX12
{
	class ResourceFactory;
	

	
	class RendererFacadeImpl final : public RendererFacade, public MaintainsInternalRenderResources
	{	
		private: UniquePtr<RHA::DX12::DeviceResources> resources;

		private: UniquePtr<RHA::DX12::Queue> commonQueue;
		
		private: UniquePtr<RHA::DX12::Fence> closeFence;

		private: HANDLE closeEvent;
				 			
		private: UniquePtr<ResourceFactory> resourceFactory;
		
		private: ResourceRegistry registry;

		private: BlendSettingsImpl blendSettings;

		private: DepthStencilSettingsImpl depthStencilSettings;

		private: VertexLayoutSettingsImpl vertexLayoutSettings;

		private: RasterizerSettingsImpl rasterizerSettings;
		
		private: PsoFactory psoFactory;
		
		private: RootSignatureFactory signatureFactory;
		
		private: UniquePtr<ShaderFactory> shaderFactory;
			
		private: DescriptorMemory descriptors;

		private: Commands::CommandProcessorImpl commandProcessor;

		private: ResourceViewFactoryImpl resourceViewFactory;

	 			
		
		public: RendererFacadeImpl(HWND outputWindow);
											 
		public: ~RendererFacadeImpl();

			private: void WaitForIdleQueue();

		public: RendererFacadeImpl(const RendererFacadeImpl &) = delete;

		public: RendererFacadeImpl &operator=(const RendererFacadeImpl &) = delete;


		public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes) override;

			private: size_t MakeBufferInternal(const void *data, size_t sizeInBytes, size_t handle);
		
		public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes, D3D12_RESOURCE_STATES state) override;

		public: virtual size_t MakeUavBuffer(const void *data, size_t sizeInBytes) override;

		public: virtual DxPtr<ID3D12Resource> MakeReadbackBuffer(size_t sizeInBytes) override;

		
		public: virtual void CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const override;

		public: virtual void CompilePixelShader(const char *shader, size_t length, SerializationHook *serializer) const override;

		public: virtual void CompileComputeShader(const char *shader, size_t length, SerializationHook *serializer) const override;
		

		public: virtual void SerializeRootSignature
		(
			unsigned cbvAmount,
			unsigned srvAmount,
			unsigned uavAmount,
			unsigned samplerAmount,
			SerializationHook *serializer,
			const SamplerSpec *staticSamplers,
			unsigned numStaticSamplers
		)	override;

		public: virtual size_t MakeRootSignature(const void *serializedData) override;
							
			private: static SIZE_T ExtractSizeFrom(const void *data);

			private: static const unsigned char *ExtractSignatureFrom(const void *data);

			private: static size_t ExtractSamplerCountFrom(const void *data, SIZE_T signatureSize);
		
		public: size_t MakePso(const ShaderList &shaders, size_t signatureHandle) override;
							
		public: size_t MakePso(const Blob &csBlob, size_t signatureHandle) override;

		public: ResourceHandle::t_hash MakeTexture(const void *data, size_t width, size_t height) override;

		
		public: bool IsResourceValid(size_t handle) override;

		public: void RetireHandle(size_t handle) override;

   		
		public: ResourceHandle::t_hash MakeWindowsWindowSurface(HWND windowHandle) override;

		public: void SetWindowSurfaceToFullscreen(ResourceHandle::t_hash surface) override;

			private: void WaitForCommandsAndQueue();
		
		public: void SetWindowSurfaceToWindowed(ResourceHandle::t_hash surface) override;
		
		public: void FitWindowSurfaceToWindow(ResourceHandle::t_hash surface) override;

		
		public: UniquePtr<::Renderer::Commands::CommandFactory> MakeCommandFactory() override;

		public: void SubmitCommand(UniquePtr<::Renderer::Commands::Command> &&command) override;

		public: void SubmitContextCommand(UniquePtr<::Renderer::Commands::Command> &&command) override;
		
		public: void DestroyUnreferencedResources() override;

		public: void DestroyExecutedCommands() override;

		
		public: BlendSettings &GetBlendSettings() override;
		
		public: DepthStencilSettings &GetDepthStencilSettings() override;

		public: RasterizerSettings &GetRasterizerSettings() override;
		
		public: VertexLayoutSettings &GetVertexLayoutSettings() override;

		
		public: ResourceViewFactory &GetViewFactory() override;
							   		
	};


}
