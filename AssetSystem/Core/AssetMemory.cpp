#include "AssetSystem/Core/AssetMemory.hpp"
#include "Shared/Exception/Exception.hpp"


namespace assetSystem::core
{
	void AssetMemory::RegisterAssetClass
	(
		const char *classFileExtension, 
		UniquePtr<AssetConstructionOperations> &&constructOperations
	)
	{
		if(assetConstructOperations.find(classFileExtension) == assetConstructOperations.end())
		{
			assetConstructOperations.insert( {classFileExtension, std::move(constructOperations)} );			
		}
		
	}


	
	Asset &AssetMemory::MakeAsset(Asset &&assetData, const AssetKey key, const char *classFileExtension)
	{
		auto &cOps{ assetConstructOperations.at(classFileExtension) };
		
		return cOps->ConstructAsset(AllocateAssetMemory(key, *cOps), std::move(assetData));
		
	}
	
		char *AssetMemory::AllocateAssetMemory(const AssetKey key, AssetConstructionOperations &cOps)
		{
			auto &assetInfo{ assetsInfos[key] = {MakeUnique<char[]>(cOps.GetAssetSizeInBytes()), &cOps} };

			return assetInfo.memory.get();
			
		}


	
	Asset &AssetMemory::MakeAsset(AssetKey key, const char *classFileExtension)
	{
		Exception::ThrowIfDebug(assetConstructOperations.find(classFileExtension) == assetConstructOperations.end(), {"An asset class must be registered before it can be constructed."});
		
		auto &cOps{ assetConstructOperations.at(classFileExtension) };
		
		return cOps->ConstructAsset(AllocateAssetMemory(key, *cOps));
				
	}



	Asset &AssetMemory::GetAsset(const AssetKey key)
	{
		return *reinterpret_cast<Asset *>(assetsInfos.at(key).memory.get());//todo: is type punning from char * defined behavior ?
		
	}
	

	
	void AssetMemory::FreeAsset(const AssetKey key)
	{
		auto &assetInfo{ assetsInfos.at(key) };
		
		reinterpret_cast<Asset *>(assetInfo.memory.get())->~Asset();	//todo try to replace DestructAsset with call to Asset::~Asset()
		assetsInfos.erase(key);
		
	}


	
	bool AssetMemory::IsAssetLoaded(const AssetKey key) const
	{
		return assetsInfos.find(key) != assetsInfos.end();
		
	}

	
}
