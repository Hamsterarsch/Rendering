#pragma once
#include <Windows.h>
#include <future>
#include "RendererExportHelper.hpp"
#include "Shared/PtrTypes.hpp"
#include "Resources/Pso/PipelineTypes.hpp"
#include "Resources/Pso/ShaderList.hpp"
#include "Resources/Pso/VertexLayoutTypes.hpp"
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
	class SerializationHook;


	class RENDERER_DLLSPEC Renderer
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Renderer)
			

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


	UniquePtr<Renderer> RENDERER_DLLSPEC MakeRenderer(HWND outputWindow);
	
	
}
