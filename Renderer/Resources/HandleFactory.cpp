#include "Resources/HandleFactory.hpp"


namespace Renderer
{
	ResourceHandle HandleFactory::MakeHandle(const ResourceHandle::t_resourceTypes type)
	{
		std::lock_guard<std::mutex> lock{ mutex };
		
		return ResourceHandle{ type, serialFactories[type].GetNextSerial() };
				
	}
	
	
}
