#include "AssetPtr.hpp"


namespace assetSystem
{
	AssetPtr *AssetPtr::operator->()
	{
		return this;
		
	}



	const AssetPtr *AssetPtr::operator->() const
	{
		return this;
		
	}



	AssetPtr::AssetPtr(const char *relativePath, AssetSystem &system)
		:
		asset{ nullptr },
		key{ 0 },
		assetSystem{ &system }
	{
		if(const auto loadInfo{ assetSystem->GetAsset(relativePath) }; loadInfo.asset)
		{
			asset = loadInfo.asset;
			key = loadInfo.key;						
		}
		
	}



	AssetPtr::AssetPtr(AssetKey key, AssetSystem &system)
		:
		asset{ nullptr },
		key{ key },
		assetSystem{ &system }
	{
		if(auto *asset{ assetSystem->GetAsset(key) })
		{
			this->asset = asset;					
		}		
		
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

		asset = rhs.asset;
		key = rhs.key;
		assetSystem = rhs.assetSystem;

		if(this->IsInvalid())
		{
			return *this;
			
		}

		assetSystem->AddReference(key);

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

	
}
