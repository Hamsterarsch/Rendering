#pragma once
#include <map>
#include "Resources/ResourceHandle.hpp"


namespace Renderer
{
	class ResourceRegistry
	{
		private: std::map<ResourceHandle, std::wstring> data;

		private: std::map<ResourceType, size_t> currentSerials;
		

		
		public: const wchar_t *GetResourceFilepath(ResourceHandle handle);

		public: ResourceHandle RegisterResource(const wchar_t *filepath);

			private: ResourceHandle MakeNewResourceHandle();

				private: static ResourceType DetermineResourceTypeFromFileending(const wchar_t *filepath);

					private: static const wchar_t *ExtractFileending(const wchar_t *filepath);

				private: size_t MakeSerialForResourceType(ResourceType type);
		
	};

	
}
