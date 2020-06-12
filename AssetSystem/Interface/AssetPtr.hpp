#pragma once
#include "AssetSystem.hpp"
#include "AssetSystemTypes.hpp"


namespace assetSystem
{	
	class AssetPtr
	{
		private: Asset *asset;

		private: AssetKey key;

		private: AssetSystem *assetSystem;

		

		public: AssetPtr *operator->();

		public: const AssetPtr *operator->() const;

		public: AssetPtr(const char *relativePath, AssetSystem &system);
		
		public: AssetPtr(AssetKey key, AssetSystem &system);
							   		
		public: AssetPtr(const AssetPtr &other);

		public: ~AssetPtr();

		public: AssetPtr &operator=(const AssetPtr &rhs);
		
		public: AssetPtr(AssetPtr &&other) noexcept;

		public: AssetPtr &operator=(AssetPtr &&rhs) noexcept;

			private: void Invalidate();

		public: bool IsInvalid() const;

				
	};

	
}
