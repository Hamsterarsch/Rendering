#pragma once
#include "AssetSystem/Interface/Asset.hpp"
#include "AssetConstructOperationsHelper.hpp"
#include "AssetSystem.hpp"
#include "Windows/Application.hpp"


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
				
				Windows::Application::Get().GetProgramAssets().RegisterAssetClass(classExtension, MakeUnique<assetSystem::AssetConstructOperationsHelper<t_subclass>>());
				Windows::Application::Get().GetProgramAssets().RegisterAssetClass(classExtension, MakeUnique<assetSystem::AssetConstructOperationsHelper<t_subclass>>());
				wasRegistered = true;				
			}
			
		}
		
	};

	
}
