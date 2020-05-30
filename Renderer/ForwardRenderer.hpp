#pragma once
#include "Interface/RendererFacade.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Resources/Pso/PsoFactory.hpp"
#include "Resources/RootSignature/RootSignatureFactory.hpp"
#include "DX12/ShaderFactory.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "ShaderRelevantTypes/GlobalBufferData.hpp"
#include "Lighting/LightGrid/VolumeTileGrid.hpp"
#include "Commands/CommandHelper.hpp"
#include "Commands/CommandProcessorImpl.hpp"
#include "Resources/HandleWrapper.hpp"
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
	

	
	class ForwardRenderer final : public RendererFacade, public MaintainsInternalRenderResources
	{	
		private: long long lastDispatchTime;

		private: const unsigned char maxScheduledFrames;
		
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
		


		private: HandleWrapper globalBuffer;

		private: GlobalBufferData globalsToDispatch;

		private: DescriptorMemory descriptors;

		private: HandleWrapper depthOnlyPso, defaultSignature, uav1Signature, markActiveTilesPso, markActiveTilesSignature, buildTileListPso, buildTileListSignature;

		private: HandleWrapper assignLightsSignature, assignLightsPso, lightsBuffer;

		private: VolumeTileGrid volumeTileGrid;

		private: CommandHelper cmdFactory;

		private: Commands::CommandProcessorImpl commandProcessor;

		private: UniquePtr<Commands::InitVolumeTileGridCommand> initGridCmd;

		private: std::vector<Commands::RenderMeshArguments> opaqueMeshArguments;

		private: ResourceViewFactoryImpl resourceViewFactory;

	 			
		
		public: ForwardRenderer(HWND outputWindow);
											 
		public: ~ForwardRenderer();

			private: void WaitForIdleQueue();


		public: virtual bool IsBusy() const override;
		
		public: virtual void DispatchFrame() override;
							
			private: void AbortDispatch();
					 		

		public: virtual void RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices, size_t transformBufferHandle = 0, size_t instanceCount = 1) override;

		public: virtual void SetCamera(float x, float y, float z, float pitch, float yaw, float roll) override;
							

		public: virtual size_t MakeLight(const float (& position)[3], const float (& rotation)[3], const float(& color)[3], float radius) override;
				
		
		public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes) override;

			private: size_t MakeBufferInternal(const void *data, size_t sizeInBytes, size_t handle);
					 			
		public: virtual void RemakeBuffer(const void *data, size_t sizeInBytes, size_t handle) override;
		

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

		
		public: bool ResourceMustBeRemade(size_t handle) override;

		public: void RetireHandle(size_t handle) override;


		//new interface
							   		
		public: ResourceHandle::t_hash MakeWindowsWindowSurface(HWND windowHandle) override;

		public: UniquePtr<::Renderer::Commands::CommandFactory> MakeCommandFactory() override;

		public: void SubmitCommand(UniquePtr<::Renderer::Commands::Command> &&command) override;

		public: void SubmitContextCommand(UniquePtr<::Renderer::Commands::Command> &&command) override;
		
		public: void DestroyUnreferencedResources() override;

		public: void DestroyExecutedCommands() override;

		public: void WaitForCommands() override;

		
		public: BlendSettings &GetBlendSettings() override;
		
		public: DepthStencilSettings &GetDepthStencilSettings() override;

		public: RasterizerSettings &GetRasterizerSettings() override;
		
		public: VertexLayoutSettings &GetVertexLayoutSettings() override;

		
		public: ResourceViewFactory &GetViewFactory() override;

		
	};


}
