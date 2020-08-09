#pragma once
#include "Shared/Exception/Exception.hpp"
#include "Resources/ResourceHandle.hpp"


namespace Renderer::Exceptions
{												
	class InvalidRegistryHandleException final : public Exception::Exception				
	{
		public: ResourceHandle::t_hash handleAtFault;


		
		public: InvalidRegistryHandleException(ResourceHandle::t_hash handleAtFault, const char *description)
			:	
			Exception(description),
			handleAtFault{ handleAtFault }			
		{}										
												
	};

	
}
