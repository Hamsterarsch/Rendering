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
		private: Asset *asset;

		private: AssetKey key;

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


		public: Asset *GetAsset() { return asset; }

		public: const Asset *GetAsset() const { return asset; }
		
		public: AssetKey GetKey() const { return key; }

		public: Asset *operator->() { return asset; }

		public: const Asset *operator->() const { return asset; }

		public: void SaveToDisk();

		public: void LoadFromDisk();
								
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
