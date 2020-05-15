#include "Lighting/LightContainer.hpp"


namespace Renderer
{
	void LightContainer::Add(const ResourceHandle::t_hash handle, Light &&light)
	{
		lights.emplace_back(std::move(light));
		indices[handle] = lights.size()-1;
		
	}


	
	void LightContainer::Remove(const ResourceHandle::t_hash handle)
	{
		if(lights.size() == 1)
		{
			lights.clear();
			indices.clear();
			handles.clear();
		}
		
		const auto indexItrToDelete{ indices.find(handle) };

		const auto &replacementLightHandle{ handles.at(lights.size()-1) };

		lights.at(indexItrToDelete->second) = std::move(lights.back());
		indices.at(replacementLightHandle) = indexItrToDelete->second;

		lights.pop_back();
		indices.erase(indexItrToDelete);
				
	}


	
	Light &LightContainer::Get(const ResourceHandle::t_hash handle)
	{		
		return lights.at(indices.at(handle));
		
	}


	
	size_t LightContainer::Size() const
	{
		return lights.size();
		
	}


	
	size_t LightContainer::SizeInBytes() const
	{
		return lights.size() * sizeof(decltype(lights)::value_type);
		
	}


	
	const void *LightContainer::GetData() const
	{
		return lights.data();
		
	}


		
	bool LightContainer::IsHandleUnknown(const ResourceHandle::t_hash handle) const
	{
		return indices.find(handle) == indices.end();
		
	}
	
	
}
