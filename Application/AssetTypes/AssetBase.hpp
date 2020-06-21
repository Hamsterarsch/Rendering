#pragma once
#include "AssetSystem/Interface/Asset.hpp"
#include "AssetConstructOperationsHelper.hpp"
#include "Core/Globals.hpp"
#include "AssetSystem.hpp"


namespace App::Assets
{
	template<class t_subclass>
	class AssetBase : public assetSystem::Asset
	{
		inline static bool wasRegistered{ false };
		
		public: AssetBase(const char *classExtension)
		{
			if(not wasRegistered)
			{
				Core::GetProgramAssets().RegisterAssetClass(classExtension, MakeUnique<assetSystem::AssetConstructOperationsHelper<t_subclass>>());
				Core::GetProjectAssets().RegisterAssetClass(classExtension, MakeUnique<assetSystem::AssetConstructOperationsHelper<t_subclass>>());
				wasRegistered = true;				
			}
			
		}
		
	};

	
}