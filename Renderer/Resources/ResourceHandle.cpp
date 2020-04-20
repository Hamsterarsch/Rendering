#include "Resources/ResourceHandle.hpp"


namespace Renderer
{
	ResourceHandle::ResourceHandle(const t_resourceTypes type, const t_serial serial) :
		hash{ MakeHash(type, serial) }
	{						
	}

		size_t ResourceHandle::MakeHash(const t_resourceTypes type, const t_serial serial)
		{
			constexpr size_t typeErasure{ ~size_t(0) >> sizeof t_resourceTypes* 8	};
					
			size_t typeMask{ static_cast<unsigned char>(type) };
			typeMask = typeMask << (sizeof t_serial - sizeof t_resourceTypes) * 8;
		
			return (serial & typeErasure) | typeMask;
			
		}


	
	ResourceHandle::ResourceHandle(const t_hash hash) :
		hash{ hash }
	{
	}


	
	ResourceHandle::t_resourceTypes ResourceHandle::GetResourceType() const
	{
		return GetResourceType(hash);
			
	}

		ResourceHandle::t_resourceTypes ResourceHandle::GetResourceType(t_hash hash)
		{
			return static_cast<t_resourceTypes>(hash >> (sizeof t_serial - sizeof t_resourceTypes) * 8);
		
		}


	
	size_t ResourceHandle::GetSerial() const
	{
		return (hash << sizeof t_resourceTypes * 8) >> sizeof t_resourceTypes * 8;
		
	}

	
}
