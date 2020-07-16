#pragma once
#include "AssetSystemExportHelper.hpp"
#include "AssetSystemTypes.hpp"
#include <utility>


namespace assetSystem
{
	class AssetSystem;
	class Asset;
	
	class ASSET_SYSTEM_DLLSPEC AssetPtr
	{		
		private: mutable Asset *cachedAsset;

		private: mutable AssetKey cachedKey;

		private: AssetSystem *assetSystem;

		
		
		public: AssetPtr();
		
		public: AssetPtr(const char *projectRelativePath, AssetSystem &system);

		public: AssetPtr(Asset &asset, AssetKey key, AssetSystem &system);
							   		
		public: AssetPtr(const AssetPtr &other);

		public: ~AssetPtr();

		public: AssetPtr &operator=(const AssetPtr &rhs);
		
		public: AssetPtr(AssetPtr &&other) noexcept;

		public: AssetPtr &operator=(AssetPtr &&rhs) noexcept;

			private: void Invalidate();

		public: bool IsInvalid() const;
		

		public: Asset *operator->() { return GetAsset(); }

		public: const Asset *operator->() const { return GetAsset(); }

		public: Asset *GetAsset() const;

			private: void UpdateCache() const;
					
		
		public: void SaveToDisk();

		public: void LoadFromDisk();

		public: bool operator!=(const AssetPtr &rhs) const;
		
		public: bool operator==(const AssetPtr &rhs) const;

			private: AssetKey GetKey() const;
					 		
	};

	

	template<class t_asset>
	class AssetPtrTyped : public AssetPtr
	{
		public: AssetPtrTyped() = default;

		public: AssetPtrTyped(const AssetPtr &other) : AssetPtr{ other } {}

		public: AssetPtrTyped(AssetPtr &&other) : AssetPtr{ std::move(other) } {}

		
		
		public: t_asset *GetAsset(){ return reinterpret_cast<t_asset *>(AssetPtr::GetAsset()); }

		public: const t_asset *GetAsset() const { return reinterpret_cast<const t_asset *>(AssetPtr::GetAsset()); }

		public: t_asset *operator->() { return GetAsset(); }

		public: const t_asset *operator->() const { return GetAsset(); }
							   		
	};
	
	
}
