#include "Resources/HandleFactory.hpp"


namespace Renderer
{
	HandleBucket::HandleBucket() :
		size{ 0 }
	{
	}


	
	ResourceHandle HandleBucket::PopHandle()
	{
		decltype(items)::value_type item{ std::move(items.front()) };

		items.pop_front();
		--size;

		return item;
		
	}

	
	
	void HandleBucket::PushHandle(const ResourceHandle &handle)
	{
		items.push_front(handle);
		++size;
				
	}


	
	size_t HandleBucket::Size() const
	{
		return size;
		
	}


	
	bool HandleBucket::IsNotEmpty() const
	{
		return !IsEmpty();
		
	}
	
		bool HandleBucket::IsEmpty() const
		{
			return size <= 0;
			
		}

	

	
	ResourceHandle HandleFactory::MakeHandle(const ResourceHandle::t_resourceTypes type)
	{
		if(retiredHandles[type].IsNotEmpty())
		{
			return retiredHandles[type].PopHandle();
		}
		
		return ResourceHandle{ type, serialFactories[type].GetNextSerial() };
				
	}

	void HandleFactory::RetireHandle(const ResourceHandle &handle)
	{
		retiredHandles[handle.GetResourceType()].PushHandle(handle);		
				
	}

	
}
