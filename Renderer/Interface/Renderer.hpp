#pragma once
#include <Windows.h>
#include <condition_variable>
#include <future>
#include "RendererExportHelper.hpp"
#include "Shared/PtrTypes.hpp"
#include "Resources/Pso/PipelineTypes.hpp"
#include "Resources/Pso/ShaderList.hpp"
#include "Resources/Pso/VertexLayoutTypes.hpp"


#include "DxPtrTypes.hpp"



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
	}
}

struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Resource;

namespace Renderer
{
	namespace DX12
	{
		struct TriangleData;

		class SerializationHook
		{
			public: virtual void SerializeData(const void *data, size_t sizeInBytes) {};

			//interface functions
		};
						
		class RENDERER_DLLSPEC Renderer
		{
			
			private: const unsigned inflightFramesAmount;
			
			private: std::mutex updaterMutex;

			private: std::condition_variable updaterCondition;

			private: bool shouldUpdateRendering;
			
			private: std::future<int> updaterHandle;

			private: UniquePtr<RHA::DX12::DeviceResources> resources;

			private: UniquePtr<RHA::DX12::Queue> commonQueue;

			private: UniquePtr<RHA::DX12::CmdAllocator> commonAllocator;
			
			private: UniquePtr<RHA::DX12::WindowSurface> outputSurface;

			private: UniquePtr<RHA::DX12::Fence> closeFence;

			private: HANDLE closeEvent;

			private: UniquePtr<class ResourceFactory> resourceFactory;
			
					 UniquePtr<TriangleData> data;
					 DxPtr<ID3D12RootSignature> signature;
					 DxPtr<ID3D12PipelineState> pipeline;
					 UniquePtr<RHA::DX12::CmdList> list;
					 UniquePtr<class ResourceAllocation> meshBufferAllocation;
						
			private: struct PrivateMembers;

			private: UniquePtr<PrivateMembers> privateMembers;
			
					 		
			
			public: Renderer(HWND outputWindow);

				private: int UpdateRendering();

			public: ~Renderer();


			
			public: size_t MakeAndUploadBufferResource(const void *data, size_t sizeInBytes);

			public: void CompileVertexShader(const char *shader, size_t length, SerializationHook *serializer) const;


			public: void SerializeRootSignature(unsigned cbvAmount, unsigned srvAmount, unsigned uavAmount, unsigned samplerAmount, SerializationHook *serializer);

			public: size_t MakeRootSignature(const void *serializedData, size_t dataLength);
			
			public: size_t MakePso(PipelineTypes pipelineType, VertexLayoutTypes vertexLayout, const ShaderList &shaders, size_t signatureHandle);

			
			

			public: bool ResourceHasToBeReloaded(size_t handle);
			
			public: void MakeBufferWithHandle(const void *data, size_t sizeInBytes, size_t handle);


			public: void RenderMesh(size_t meshHandle, size_t sizeInBytes, size_t byteOffsetToIndices);

			public: void DispatchFrame();
			
			
			
			public: void SubmitFrameInfo();

			public: void WaitForCapacity();

			
			
		};

		
	}
	
	
}
