#pragma once
#include "Interface/Renderer.hpp"


namespace Renderer
{
	namespace DX12
	{
		class FrameRenderer;
		
		class ForwardRenderer : public Renderer
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

			
			
			public: ForwardRenderer(HWND outputWindow);
												 
			public: ~ForwardRenderer();

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
	
	
}
