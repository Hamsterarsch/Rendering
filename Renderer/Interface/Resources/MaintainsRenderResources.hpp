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

		
		public: virtual size_t MakeBuffer(const void *data, size_t sizeInBytes) = 0;

		public: virtual size_t MakeRootSignature(const void *serializedData, size_t dataSizeInBytes, unsigned samplerAmount) = 0;
					
		public: virtual size_t MakePso(const ShaderList &shaders, ResourceHandle::t_hash signatureHandle) = 0;
					
		public: virtual size_t MakePso(const Blob &csBlob, ResourceHandle::t_hash signatureHandle) = 0;

		public: virtual ResourceHandle::t_hash MakeTexture(const void *data, size_t width, size_t height) = 0;

		
		public: virtual bool IsResourceValid(ResourceHandle::t_hash handle) = 0;
					
		public: virtual void RetireHandle(ResourceHandle::t_hash handle) = 0;
		
	};

	
}
