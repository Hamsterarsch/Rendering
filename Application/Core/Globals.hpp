#pragma once
#include "Shared/PtrTypes.hpp"
#include "Version.hpp"


namespace assetSystem
{
	class AssetSystem;
	
}


namespace App::Core
{
	struct Globals
	{
		const Version programVersion;

		UniquePtr<assetSystem::AssetSystem> programAssetSystem;

		UniquePtr<assetSystem::AssetSystem> projectAssetSystem;

		

		Globals();

		Globals(const Globals &) = delete;

		Globals &operator=(const Globals &) = delete;
				
		Globals(Globals &&) = delete;

		Globals &operator=(Globals &&) = delete;		
		
		~Globals();
		
	};

	inline static Globals globals{};
	
}
