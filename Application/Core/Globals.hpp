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
		private: UniquePtr<assetSystem::AssetSystem> programAssetSystem;

		public: const Version programVersion;

		public: UniquePtr<assetSystem::AssetSystem> projectAssetSystem;

		
		friend assetSystem::AssetSystem &GetProgramAssets();
			   		
		Globals();

		Globals(const Globals &) = delete;

		Globals &operator=(const Globals &) = delete;
				
		Globals(Globals &&) = delete;

		Globals &operator=(Globals &&) = delete;		
		
		~Globals();
		
	};
	inline static Globals globals{};

	
	inline assetSystem::AssetSystem &GetProgramAssets()
	{
		return *globals.programAssetSystem;
		
	}

	inline assetSystem::AssetSystem &GetProjectAssets()
	{
		return *globals.projectAssetSystem;
		
	}
	
	
}
