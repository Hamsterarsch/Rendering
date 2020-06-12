#pragma once
#include "AssetSystem.hpp"
#include "AssetRegistry.hpp"


namespace assetSystem::Core
{
	class AssetSystemImpl final : public AssetSystem
	{
		private: AssetRegistry registry;


		public: AssetSystemImpl(AssetRegistry &&registry);
		
	};
	
}
