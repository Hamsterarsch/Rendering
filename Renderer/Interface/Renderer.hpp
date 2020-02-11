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
	namespace DX12
	{
		class FrameRenderer;
		struct TriangleData;

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
						
		class RENDERER_DLLSPEC Renderer
		{	
			private: bool shouldUpdateRendering;
			
			private: std::future<int> updaterHandle;

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

			private: float shaderTimer;

					 		
			
			public: Renderer(HWND outputWindow);

				private: int UpdateRendering();

					private: void UpdateIdle();
			
					private: void ExecuteNextFrame();

					private: void WaitForIdleQueue();
							 
			public: ~Renderer();

			
			public: void DispatchFrame();

				private: bool NextFrameSlotIsOccupied() const;

				private: void AbortDispatch();

				private: FrameRenderer MakeFrameFromCommands();

			public: void RenderMesh(size_t signatureHandle, size_t psoHandle, size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices);

			public: void SetCamera(float x, float y, float z, float pitch, float yaw, float roll);
								
			
			
			public: size_t MakeBuffer(const void *data, size_t sizeInBytes);

			public: void RemakeBuffer(const void *data, size_t sizeInBytes, size_t handle);
			
			
			public: void CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const;

			public: void CompilePixelShader(const char *shader, size_t length, SerializationHook *serializer) const;
			

			public: void SerializeRootSignature(unsigned cbvAmount, unsigned srvAmount, unsigned uavAmount, unsigned samplerAmount, SerializationHook *serializer);

			public: size_t MakeRootSignature(const void *serializedData);
								
				private: static SIZE_T ExtractSizeFrom(const void *data);

				private: static const unsigned char *ExtractSignatureFrom(const void *data);

				private: static size_t ExtractSamplerCountFrom(const void *data, SIZE_T signatureSize);
			
			public: size_t MakePso(PipelineTypes pipelineType, VertexLayoutTypes vertexLayout, const ShaderList &shaders, size_t signatureHandle);
								

			public: bool ResourceMustBeRemade(size_t handle);
			
									   					 											
		};

		
	}
	
	
}
