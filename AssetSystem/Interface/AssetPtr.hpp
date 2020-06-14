#pragma once
#include "AssetSystemExportHelper.hpp"
#include "AssetSystemTypes.hpp"


namespace assetSystem
{
	class AssetSystem;
	class Asset;
	
	class ASSET_SYSTEM_DLLSPEC AssetPtr
	{		
		private: Asset *asset;

		private: AssetKey key;

		private: AssetSystem *assetSystem;

		

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
								
	};
	
	
}
