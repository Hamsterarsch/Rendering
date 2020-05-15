#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/Pso/PipelineTypes.hpp"
#include "Resources/Pso/ShaderList.hpp"
#include "Resources/Pso/VertexLayoutTypes.hpp"


namespace Renderer
{
	class RENDERER_DLLSPEC MaintainsRenderResources
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(MaintainsRenderResources)

		
		public: virtual size_t MakeLight(float x, float y, float z, float pitch, float yaw, float roll) = 0;
		
		
		public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes) = 0;

		public: virtual void RemakeBuffer(const void *data, size_t sizeInBytes, size_t handle) = 0;
		
		public: virtual size_t MakeRootSignature(const void *serializedData) = 0;
					
		public: virtual size_t MakePso(PipelineTypes pipelineType, VertexLayoutTypes vertexLayout, const ShaderList &shaders, size_t signatureHandle) = 0;
					
		public: virtual size_t MakePso(const Blob &csBlob, size_t signatureHandle) = 0;

		
		public: virtual bool ResourceMustBeRemade(size_t handle) = 0;
					
		public: virtual void RetireHandle(size_t handle) = 0;
		
	};

	
}
