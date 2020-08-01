#pragma once
#include <unordered_map>
#include "AssetSystemTypes.hpp"


namespace assetSystem::core
{
	class InMemoryReferences
	{
		private: struct ReferenceBucket
		{
			unsigned strongReferences{ 0 };			
			
		};

		private: std::unordered_map<AssetKey, ReferenceBucket> references;



		public: void AddReference(AssetKey to);

		public: void RemoveReference(AssetKey to);

		public: bool HasNoReferences(AssetKey key) const;

		public: bool HasReferences(AssetKey key) const;

		public: void AddSourceReferenceCountToTarget(AssetKey source, AssetKey target);
		
		
	};

	
}
