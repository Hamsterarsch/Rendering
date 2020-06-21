#pragma once
#include "Core/Globals.hpp"
#include "AssetConstructOperationsHelper.hpp"
#include "AssetSystem.hpp"


namespace App::AssetTypes
{
	template<class t_asset>
	void RegisterAssetClass(const char *classExtension, bool &isAlreadyRegistered)
	{
		if(not isAlreadyRegistered)
		{
			GetProgramAssets().RegisterAssetClass(classExtension, MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
			GetProjectAssets().RegisterAssetClass(classExtension, MakeUnique<assetSystem::AssetConstructOperationsHelper<t_asset>>());
			isAlreadyRegistered = true;
		}
		
	}

	
}
