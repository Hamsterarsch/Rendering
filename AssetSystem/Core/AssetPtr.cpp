#include "AssetPtr.hpp"
#include "AssetSystem.hpp"


namespace assetSystem
{
	AssetPtr::AssetPtr() : asset{ nullptr }, key{ 0 }, assetSystem { nullptr } {}


	
	AssetPtr::AssetPtr(const char *projectRelativePath, AssetSystem &system)
		:
		asset{ nullptr },
		key{ 0 },
		assetSystem{ &system }
	{
		if(const auto loadInfo{ assetSystem->GetAssetInternal(projectRelativePath) }; loadInfo.asset)
		{
			asset = loadInfo.asset;
			key = loadInfo.key;						
		}
		
	}


	
	AssetPtr::AssetPtr(Asset &asset, const AssetKey key, AssetSystem &system)
		:
		asset{ &asset },
		key{ key },
		assetSystem{ &system }
	{
	}



	AssetPtr::AssetPtr(const AssetPtr &other)
		:
		asset{ nullptr },
		key{ 0 },
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

		assetSystem->RemoveReference(key);
		
	}



	AssetPtr &AssetPtr::operator=(const AssetPtr &rhs)
	{
		if(this == &rhs)
		{
			return *this;
			
		}

		const auto pointToDifferentAssets{ this->key != rhs.key };
		if(!this->IsInvalid() && pointToDifferentAssets)
		{
			assetSystem->RemoveReference(key);
		}
		
		asset = rhs.asset;
		key = rhs.key;
		assetSystem = rhs.assetSystem;

		if(!this->IsInvalid() && pointToDifferentAssets)
		{
			assetSystem->AddReference(key);			
		}
		
		return *this;
		
	}


	
	AssetPtr::AssetPtr(AssetPtr &&other) noexcept
		:
		asset{ nullptr },
		key{ 0 },
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

		if(!this->IsInvalid() && this->key != rhs.key)
		{
			assetSystem->RemoveReference(key);
		}
		
		asset = std::move(rhs.asset);
		key = std::move(rhs.key);
		assetSystem = std::move(rhs.assetSystem);

		rhs.Invalidate();

		return *this;
				
	}

		void AssetPtr::Invalidate()
		{
			asset = nullptr;
			key = 0;
		
		}


	
	bool AssetPtr::IsInvalid() const
	{
		return asset == nullptr;
		
	}


	
	void AssetPtr::SaveToDisk()
	{
		assetSystem->SerializeAsset(*this);
		
	}


	
	void AssetPtr::LoadFromDisk()
	{
		assetSystem->ReloadAsset(*this);
		
	}

	
}
