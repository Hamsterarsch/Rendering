#include "AssetPtr.hpp"
#include "AssetSystem.hpp"


namespace assetSystem
{
	AssetPtr::AssetPtr() : cachedAsset{ nullptr }, cachedKey{ 0 }, assetSystem { nullptr } {}


	
	AssetPtr::AssetPtr(const char *projectRelativePath, AssetSystem &system)
		:
		cachedAsset{ nullptr },
		cachedKey{ 0 },
		assetSystem{ &system }
	{
		if(const auto loadInfo{ assetSystem->GetAssetInternal(projectRelativePath) }; loadInfo.asset)
		{
			cachedAsset = loadInfo.asset;
			cachedKey = loadInfo.key;						
		}
		
	}


	
	AssetPtr::AssetPtr(Asset &asset, const AssetKey key, AssetSystem &system)
		:
		cachedAsset{ &asset },
		cachedKey{ key },
		assetSystem{ &system }
	{
	}



	AssetPtr::AssetPtr(const AssetPtr &other)
		:
		cachedAsset{ nullptr },
		cachedKey{ 0 },
		assetSystem{ nullptr }
	{
		*this = other;
		
	}



	AssetPtr::~AssetPtr()
	{
		if(IsInvalid())
		{
			return;
			
		}

		assetSystem->RemoveReference(GetCurrentKey());
		
	}



	AssetPtr &AssetPtr::operator=(const AssetPtr &rhs)
	{
		if(this == &rhs)
		{
			return *this;
			
		}

		if(not this->IsInvalid())
		{
			assetSystem->RemoveReference(GetCurrentKey());
		}
		
		cachedAsset = rhs.cachedAsset;
		cachedKey = rhs.cachedKey;
		assetSystem = rhs.assetSystem;

		if(not this->IsInvalid())
		{
			assetSystem->AddReference(GetCurrentKey());			
		}
		
		return *this;
		
	}


	
	AssetPtr::AssetPtr(AssetPtr &&other) noexcept
		:
		cachedAsset{ nullptr },
		cachedKey{ 0 },
		assetSystem{ nullptr }
	{
		*this = std::move(other);
		
	}


	
	AssetPtr &AssetPtr::operator=(AssetPtr &&rhs) noexcept
	{
		if(this == &rhs)
		{
			return *this;
			
		}

		if(!this->IsInvalid() && this->GetCurrentKey() != rhs.GetCurrentKey())
		{
			assetSystem->RemoveReference(cachedKey);
		}
		
		cachedAsset = std::move(rhs.cachedAsset);
		cachedKey = std::move(rhs.cachedKey);
		assetSystem = std::move(rhs.assetSystem);

		rhs.Invalidate();

		return *this;
				
	}

		void AssetPtr::Invalidate()
		{
			cachedAsset = nullptr;
			cachedKey = 0;
		
		}

		AssetKey AssetPtr::GetCurrentKey() const
		{
			UpdateCache();

			return cachedKey;
			
		}


	
	bool AssetPtr::IsInvalid() const
	{
		return cachedAsset == nullptr;
		
	}



	Asset *AssetPtr::GetAsset() const
	{
		UpdateCache();
		
		return cachedAsset;
	}
	
		void AssetPtr::UpdateCache() const
		{
			if(assetSystem->AssetWasInvalidated(cachedKey))
			{
				auto updatedData{ assetSystem->GetUpdatedAssetData(cachedKey) };
				
				cachedAsset = updatedData.asset;
				cachedKey = updatedData.key;
			}
		
		}
			   		 	  

	
	void AssetPtr::SaveToDisk()
	{
		assetSystem->SerializeAsset(*this);
		
	}


	
	void AssetPtr::LoadFromDisk()
	{
		assetSystem->ReloadAsset(*this);
		
	}



	bool AssetPtr::operator!=(const AssetPtr &rhs) const
	{
		return not (*this == rhs);
		
	}
	
		bool AssetPtr::operator==(const AssetPtr &rhs) const
		{
			return GetCurrentKey() == rhs.GetCurrentKey();
			
		}
							   
				
}
