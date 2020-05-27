#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Resources/Pso/PipelineTypes.hpp"
#include "Resources/Pso/ShaderList.hpp"
#include "ResourceHandle.hpp"


namespace Renderer
{
	class RENDERER_DLLSPEC MaintainsRenderResources
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(MaintainsRenderResources)

		
		public: virtual size_t MakeLight(const float (& position)[3], const float (& rotation)[3], const float(& color)[3], float radius) = 0;
		
		
		public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes) = 0;

		public: virtual void RemakeBuffer(const void *data, size_t sizeInBytes, size_t handle) = 0;
		
		public: virtual size_t MakeRootSignature(const void *serializedData) = 0;
					
		public: virtual size_t MakePso(const ShaderList &shaders, size_t signatureHandle) = 0;
					
		public: virtual size_t MakePso(const Blob &csBlob, size_t signatureHandle) = 0;

		public: virtual ResourceHandle::t_hash MakeTexture(const void *data, size_t width, size_t height) = 0;

		
		public: virtual bool ResourceMustBeRemade(size_t handle) = 0;
					
		public: virtual void RetireHandle(size_t handle) = 0;
		
	};

	
}
