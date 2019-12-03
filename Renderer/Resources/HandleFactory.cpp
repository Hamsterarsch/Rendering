#include "Resources/HandleFactory.hpp"


namespace Renderer
{
	ResourceHandle HandleFactory::MakeHandle(const ResourceHandle::t_resourceTypes type)
	{
		return ResourceHandle{ type, serialFactories[type].GetNextSerial() };
				
	}
	
	
}
