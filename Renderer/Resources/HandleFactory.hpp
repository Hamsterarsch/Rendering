#pragma once
#include "Resources/SerialFactory.hpp"
#include "Resources/ResourceHandle.hpp"
#include <unordered_map>
#include <forward_list>


namespace Renderer
{
	class HandleBucket
	{		
		private: std::forward_list<ResourceHandle> items;

		private: size_t size;
				 					

		
		public: HandleBucket();

		public: ResourceHandle PopHandle();
						
		public: void PushHandle(const ResourceHandle &handle);

		public: size_t Size() const;

		public: bool IsNotEmpty() const;

		public: bool IsEmpty() const;
				
	};


	
	class HandleFactory
	{	
		private: std::unordered_map<ResourceHandle::t_resourceTypes, SerialFactory> serialFactories;

		private: std::unordered_map<ResourceHandle::t_resourceTypes, HandleBucket> retiredHandles;
		
				 		

		public: ResourceHandle MakeHandle(ResourceHandle::t_resourceTypes type);

		public: void RetireHandle(const ResourceHandle &handle);
		
	};

	
}
