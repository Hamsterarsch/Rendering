#pragma once
#include "Resources/SerialFactory.hpp"
#include "Resources/ResourceHandle.hpp"
#include <unordered_map>
#include <mutex>


namespace Renderer
{
	class HandleFactory
	{
		private: std::unordered_map<ResourceHandle::t_resourceTypes, SerialFactory> serialFactories;

		private: std::mutex mutex;


		public: ResourceHandle MakeHandle(ResourceHandle::t_resourceTypes type);
		
	};

	
}
