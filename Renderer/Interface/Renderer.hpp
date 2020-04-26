#pragma once
#include <Windows.h>
#include <future>
#include "RendererExportHelper.hpp"
#include "Shared/PtrTypes.hpp"
#include "Resources/Pso/PipelineTypes.hpp"
#include "Resources/Pso/ShaderList.hpp"
#include "Resources/Pso/VertexLayoutTypes.hpp"
#include "DxPtrTypes.hpp"
#include "RenderCommand.hpp"


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
struct ID3D12Resource;


namespace Renderer
{
		class SerializationHook
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(SerializationHook)

			class Block
			{
				private: SerializationHook *parent;

				
				public: Block(SerializationHook *parent) : parent{parent} {}
				
				public: ~Block()
				{
					parent->EndBlock();
				}
				
			};

			size_t lastBlockSizeInBytes{ 0 };
			
			public: Block BeginBlock(size_t sizeInBytes)
			{
				lastBlockSizeInBytes = sizeInBytes;
				OnBeginBlock();
				
				return Block{ this };
				
			}

			protected: virtual void OnBeginBlock() = 0;
			
			protected: size_t GetBlockSize() const {  return lastBlockSizeInBytes; }

			friend Block::~Block();
			
			private: virtual void EndBlock() {};
			
			public: virtual void WriteToBlock(const void *data, size_t sizeInBytes) {};

		};
	
		class RENDERER_DLLSPEC IRenderer
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(IRenderer)
			

			public: virtual bool IsBusy() const = 0;
			
			public: virtual void DispatchFrame() = 0;

			public: virtual void RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices, size_t transformBufferHandle = 0, size_t instanceCount = 1) = 0;

			public: virtual void SetCamera(float x, float y, float z, float pitch, float yaw, float roll) = 0;
					
			public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes) = 0;

			public: virtual void RemakeBuffer(const void *data, size_t sizeInBytes, size_t handle) = 0;
			
			public: virtual void CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
						
			public: virtual void CompilePixelShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
						
			public: virtual void CompileComputeShader(const char *shader, size_t length, SerializationHook *serializer) const = 0;
						
			public: virtual void SerializeRootSignature(unsigned cbvAmount, unsigned srvAmount, unsigned uavAmount, unsigned samplerAmount, SerializationHook *serializer) = 0;
						
			public: virtual size_t MakeRootSignature(const void *serializedData) = 0;
						
			public: virtual size_t MakePso(PipelineTypes pipelineType, VertexLayoutTypes vertexLayout, const ShaderList &shaders, size_t signatureHandle) = 0;
						
			public: virtual size_t MakePso(const Blob &csBlob, size_t signatureHandle) = 0;
						
			public: virtual bool ResourceMustBeRemade(size_t handle) = 0;
						
			public: virtual void RetireHandle(size_t handle) = 0;

		};

	
	
	namespace DX12
	{
		class FrameRenderer;
		struct TriangleData;
		
		
		class Renderer : public IRenderer
		{	
			private: UniquePtr<RHA::DX12::DeviceResources> resources;

			private: UniquePtr<RHA::DX12::Queue> commonQueue;

			private: UniquePtr<RHA::DX12::WindowSurface> outputSurface;

			private: UniquePtr<RHA::DX12::Fence> closeFence;

			private: HANDLE closeEvent;

			private: UniquePtr<RHA::DX12::DepthSurface> depthSurface;

			private: UniquePtr<class ResourceFactory> resourceFactory;

			private: struct PrivateMembers;
			
			private: UniquePtr<PrivateMembers> privateMembers;

			private: long long lastDispatchTime;

			private: const unsigned char maxScheduledFrames;

			
			
			public: Renderer(HWND outputWindow);
												 
			public: ~Renderer();

				private: void WaitForIdleQueue();


			public: virtual bool IsBusy() const override;
			
			public: virtual void DispatchFrame() override;
								
				private: void AbortDispatch();

				private: FrameRenderer MakeFrameFromCommands();

			public: virtual void RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices, size_t transformBufferHandle = 0, size_t instanceCount = 1) override;

			public: virtual void SetCamera(float x, float y, float z, float pitch, float yaw, float roll) override;
								
			
			
			public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes) override;

				private: size_t MakeBufferInternal(const void *data, size_t sizeInBytes, size_t handle);
						 			
			public: virtual void RemakeBuffer(const void *data, size_t sizeInBytes, size_t handle) override;
			
			
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

	UniquePtr<IRenderer> RENDERER_DLLSPEC MakeRenderer(HWND outputWindow);
	
	
}
