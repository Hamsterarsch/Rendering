#pragma once
#include "Interface/Renderer.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Resources/Pso/PsoFactory.hpp"
#include "Resources/RootSignature/RootSignatureFactory.hpp"
#include "Resources/Pso/VertexLayoutProvider.hpp"
#include "Shared/Types/Containers/QueueConcurrent.hpp"
#include "DX12/ShaderFactory.hpp"
#include "RendererMaster.hpp"
#include "Resources/ResourceRegistry.hpp"
#include "Resources/MaintainsInternalRenderResources.hpp"
#include "Resources/Descriptor/DescriptorMemory.hpp"
#include "ShaderRelevantTypes/GlobalBufferData.hpp"
#include "Lighting/LightGrid/VolumeTileGrid.hpp"
#include "Resources/Pso/DepthStencilFactoryDefault.hpp"
#include "Commands/CommandFactory.hpp"

/*
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

struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Resource;*/

namespace Renderer::DX12
{
	class ResourceFactory;
	class FrameWorker;
	class RenderMeshCommand;
	class CommandInitVolumeTileGrid;
	
	class ForwardRenderer final : public Renderer, public MaintainsInternalRenderResources
	{	
		private: long long lastDispatchTime;

		private: const unsigned char maxScheduledFrames;
		
		private: UniquePtr<RHA::DX12::DeviceResources> resources;

		private: UniquePtr<RHA::DX12::Queue> commonQueue;

		private: UniquePtr<RHA::DX12::WindowSurface> outputSurface;

		private: UniquePtr<RHA::DX12::DepthSurface> depthSurface;
		
		private: UniquePtr<RHA::DX12::Fence> closeFence;

		private: HANDLE closeEvent;

				 			
		private: UniquePtr<ResourceFactory> bufferFactory;
		
		private: ResourceRegistry registry;

		private: VertexLayoutProvider vertexLayoutProvider;
		
		private: DepthStencilFactoryDefault dsFactory;
		
		private: PsoFactory psoFactory;
		
		private: RootSignatureFactory signatureFactory;
		
		private: UniquePtr<ShaderFactory> shaderFactory;
		
		private: QueueConcurrent<FrameWorker> framesToDestruct;

		private: std::future<int> activeFrameHandle;
					
		private: std::list<UniquePtr<RenderCommand>> commandsToDispatch;

		private: std::list<UniquePtr<RenderMeshCommand>> opaqueMeshCommands;

		private: HandleWrapper globalBuffer;

		private: GlobalBufferData globalsToDispatch;

		private: RendererMaster renderThread;
				 
		private: DescriptorMemory descriptors;

		private: HandleWrapper depthOnlyPso, defaultSignature, uav1Signature, markActiveTilesPso, markActiveTilesSignature, buildTileListPso, buildTileListSignature;

		private: HandleWrapper assignLightsSignature, assignLightsPso, lightsBuffer;

		private: UniquePtr<CommandInitVolumeTileGrid> initGridCmd;

		private: VolumeTileGrid volumeTileGrid;

		private: CommandFactory cmdFactory;
				 		
								 			
		
		public: ForwardRenderer(HWND outputWindow);
											 
		public: ~ForwardRenderer();

			private: void WaitForIdleQueue();


		public: virtual bool IsBusy() const override;
		
		public: virtual void DispatchFrame() override;
							
			private: void AbortDispatch();
					 		

		public: virtual void RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices, size_t transformBufferHandle = 0, size_t instanceCount = 1) override;

		public: virtual void SetCamera(float x, float y, float z, float pitch, float yaw, float roll) override;
							

		public: virtual size_t MakeLight(float x, float y, float z, float pitch, float yaw, float roll) override;
				
		
		public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes) override;

			private: size_t MakeBufferInternal(const void *data, size_t sizeInBytes, size_t handle);
					 			
		public: virtual void RemakeBuffer(const void *data, size_t sizeInBytes, size_t handle) override;
		

		public: virtual size_t MakeUavBuffer(const void *data, size_t sizeInBytes) override;

		public: virtual DxPtr<ID3D12Resource> MakeReadbackBuffer(size_t sizeInBytes) override;

		
		public: virtual void CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const override;

		public: virtual void CompilePixelShader(const char *shader, size_t length, SerializationHook *serializer) const override;

		public: virtual void CompileComputeShader(const char *shader, size_t length, SerializationHook *serializer) const override;
		

		public: virtual void SerializeRootSignature(unsigned cbvAmount, unsigned srvAmount, unsigned uavAmount, unsigned samplerAmount, SerializationHook *serializer) override;

		public: virtual size_t MakeRootSignature(const void *serializedData) override;
							
			private: static SIZE_T ExtractSizeFrom(const void *data);

			private: static const unsigned char *ExtractSignatureFrom(const void *data);

			private: static size_t ExtractSamplerCountFrom(const void *data, SIZE_T signatureSize);
		
		public: virtual size_t MakePso(PipelineTypes pipelineType, VertexLayoutTypes vertexLayout, const ShaderList &shaders, size_t signatureHandle) override;
							
		public: virtual size_t MakePso(const Blob &csBlob, size_t signatureHandle) override;

		public: virtual bool ResourceMustBeRemade(size_t handle) override;

		public: virtual void RetireHandle(size_t handle) override;
							
	};
	
	
}
