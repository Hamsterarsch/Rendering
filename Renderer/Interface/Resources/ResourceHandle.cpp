#include "Resources/ResourceHandle.hpp"


namespace Renderer
{
	ResourceHandle::ResourceHandle(const ResourceType type, const size_t serial) :
		type{ type },
		serial{ serial },
		hash{ MakeHash(type, serial) }
	{						
	}

	size_t ResourceHandle::MakeHash(const ResourceType type, const size_t serial)
	{
		constexpr size_t typeErasure{ ~size_t(0) >> sizeof type * 8	};
				
		size_t typeMask{ static_cast<unsigned char>(type) };
		typeMask = typeMask << (sizeof serial - sizeof type) * 8;
	
		return (serial & typeErasure) | typeMask;
		
	}

	
	
	ResourceHandle::ResourceHandle(const size_t hash) :
		type{ ExtractTypeFromHash(hash) },
		serial{ ExtractSerialFromHash(hash) },
		hash{ hash }			
	{			
	}

		ResourceType ResourceHandle::ExtractTypeFromHash(const size_t hash)
		{
			return static_cast<ResourceType>(hash >> (sizeof serial - sizeof type) * 8);
			
		}
	
		size_t ResourceHandle::ExtractSerialFromHash(const size_t hash)
		{
			return (hash << sizeof type * 8) >> sizeof type * 8;
			
		}

	
}
