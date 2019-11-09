#include "Resources/ResourceHandle.hpp"


namespace Renderer
{
	ResourceHandle::ResourceHandle(const t_resourceType type, const t_serial serial) :
		hash{ MakeHash(type, serial) }
	{						
	}

		size_t ResourceHandle::MakeHash(const t_resourceType type, const t_serial serial)
		{
			constexpr size_t typeErasure{ ~size_t(0) >> sizeof t_resourceType* 8	};
					
			size_t typeMask{ static_cast<unsigned char>(type) };
			typeMask = typeMask << (sizeof t_serial - sizeof t_resourceType) * 8;
		
			return (serial & typeErasure) | typeMask;
			
		}
			

		ResourceHandle::t_resourceType ResourceHandle::GetResourceType() const
		{
			return static_cast<t_resourceType>(hash >> (sizeof t_serial - sizeof t_resourceType) * 8);
			
		}
	
		size_t ResourceHandle::GetSerial() const
		{
			return (hash << sizeof t_resourceType * 8) >> sizeof t_resourceType * 8;
			
		}

	
}
