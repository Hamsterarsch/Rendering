#include "Core/SerializedReferences.hpp"
#include "IO/Archive.hpp"
#include <string>


namespace assetSystem::core
{
	io::Archive &SerializedReferences::Serialize(io::Archive &archive)
	{
		archive.EnterSubobject("references");
			uint32_t referencedCount{ static_cast<uint32_t>(references.size()) };
			archive.Serialize("count", referencedCount);

			if(archive.IsReading())
			{			
				for(uint32_t referencedIndex{ 0 }; referencedIndex < referencedCount; ++referencedIndex)
				{
					uint32_t referencedKey{ 0 };				
					archive
					.EnterSubobject(std::to_string(referencedIndex).c_str())
						.Serialize("key", referencedKey)
						.EnterSubobject("referencers");

							uint32_t referencerCount{ 0 };
							archive.Serialize("count", referencerCount);

							std::unordered_set<AssetKey> referencers{};
							for(uint32_t referencerIndex{ 0 }; referencerIndex < referencerCount; ++referencerIndex)
							{
								uint32_t referencerKey{ 0 };
								archive.Serialize(std::to_string(referencerIndex).c_str(), referencerKey);
								referencers.insert(referencerKey);								
							}
					
						archive.LeaveSubobject();

						references.insert({ referencedKey, std::move(referencers) });
					archive.LeaveSubobject();
				}
			}
			else
			{
				uint32_t referencedIndex{ 0 };
				for(auto &&referenced : references)
				{
					uint32_t key{ referenced.first };
					
					archive.EnterSubobject(std::to_string(referencedIndex).c_str())
					.Serialize("key", key)
					.EnterSubobject("referencers");

						uint32_t referencerCount{ static_cast<uint32_t>(referenced.second.size()) };
						archive.Serialize("count", referencerCount);

						uint32_t referencerIndex{ 0 };
						for(auto &&referencer : referenced.second)
						{
							uint32_t referencerKey{ referencer };
							archive.Serialize(std::to_string(referencerIndex).c_str(), referencerKey);
							++referencerIndex;
						}
					archive.LeaveSubobject();
					
				}
				
			}
			
		return archive.LeaveSubobject();
		
	}
	


	void SerializedReferences::AddReference(const AssetKey outerAsset, const AssetKey innerAsset)
	{
		wasChanged = true;
		references[innerAsset].insert(outerAsset);
		
	}


	
	void SerializedReferences::RemoveReference(const AssetKey outerAsset, const AssetKey innerAsset)
	{
		wasChanged = true;
		references.at(innerAsset).erase(outerAsset);
		
	}


	
	bool SerializedReferences::HasNoReferences(const AssetKey key) const
	{
		return references.find(key) == references.end();
		
	}


	
	void SerializedReferences::ForEachReference(const AssetKey toAsset, const std::function<void(AssetKey)> &onReference) const
	{
		const auto found{ references.find(toAsset) };
		if(found == references.end())
		{
			return;
			
		}
		
		for(auto &&reference : found->second)
		{
			onReference(reference);
		}
		
	}


	
	void SerializedReferences::RemoveAllReferencesFor(const AssetKey asset)
	{
		wasChanged = true;
		
		references.erase(asset);
		for(auto &&referenced : references)
		{
			referenced.second.erase(asset);
		}
		
	}
	
}
