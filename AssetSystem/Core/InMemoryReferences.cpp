#include "Core/InMemoryReferences.hpp"


namespace assetSystem::core
{	
	void InMemoryReferences::AddReference(const AssetKey to)
	{		
		++references[to].strongReferences;
		
	}


	
	void InMemoryReferences::RemoveReference(const AssetKey to)
	{		
		if(--references.at(to).strongReferences == 0)
		{
			references.erase(to);	
		}
		
	}


	
	bool InMemoryReferences::HasNoReferences(const AssetKey key) const
	{
		return not HasReferences(key);
		
	}

		bool InMemoryReferences::HasReferences(const AssetKey key) const
		{
			return references.find(key) != references.end();
			
		}


	
	void InMemoryReferences::AddSourceReferenceCountToTarget(const AssetKey source, const AssetKey target)
	{
		references[target].strongReferences += references.at(source).strongReferences;
				
	}

	
}
