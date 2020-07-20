#pragma once
#include "Archivable.hpp"
#include "AssetSystemTypes.hpp"
#include <unordered_map>
#include <unordered_set>
#include <functional>


namespace assetSystem::core
{
	class SerializedReferences final : public Archivable
	{
		private: bool wasChanged{ false };
		
		private: std::unordered_map<AssetKey, std::unordered_set<AssetKey>> references;


		
		public: io::Archive &Serialize(io::Archive &archive) override;
		
		public: bool WasDataChanged() const { return wasChanged; }

		public: void ClearChangedState() { wasChanged = false; }
		
		public: void AddReference(AssetKey outerAsset, AssetKey innerAsset);

		public: void RemoveReference(AssetKey outerAsset, AssetKey innerAsset);

		public: bool HasNoReferences(AssetKey key) const;

		public: void ForEachReference(AssetKey toAsset, const std::function<void(AssetKey)> &onReference) const;

		public: void RemoveAllReferencesFor(AssetKey asset);

				
	};

	
}
