#pragma once
#include "Resources/ResourceAllocation.hpp"
#include "Resources/ResourceHandle.hpp"
#include <map>


namespace Renderer
{
	class AllocationRegistry
	{
		private: std::map<ResourceHandle, ResourceAllocation> data;

		
		
		public: void AddEntry(const ResourceHandle &handle, const ResourceAllocation &allocation);

		public: const ResourceAllocation &GetEntry(const ResourceHandle &handle);

		public: void RemoveEntry(const ResourceHandle &handle);
		
	};
	
	
}
