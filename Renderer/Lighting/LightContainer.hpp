#pragma once
#include "Resources/ResourceHandle.hpp"
#include <unordered_map>
#include "ShaderRelevantTypes/Light.hpp"


namespace Renderer
{
	class LightContainer
	{
		private: std::unordered_map<ResourceHandle::t_hash, size_t> indices;

		private: std::unordered_map<size_t, ResourceHandle::t_hash> handles;
		
		private: std::vector<Light> lights;
				 

		
		public: void Add(ResourceHandle::t_hash handle, Light &&light);

		public: void Remove(ResourceHandle::t_hash handle);

		public: Light &Get(ResourceHandle::t_hash handle);

		public: size_t Size() const;

		public: size_t SizeInBytes() const;

		public: const void *GetData() const;

		public: bool IsHandleUnknown(ResourceHandle::t_hash handle) const;
				
	};

	
}
