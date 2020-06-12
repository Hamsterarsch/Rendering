#include "assetSystem.hpp"
#include "Core/AssetRegistry.hpp"


namespace assetSystem
{	
	UniquePtr<AssetSystem> MakeAssetSystem(const char *projectAssetFolder)
	{		
		AssetRegistry{ projectAssetFolder };

		
		return{};
		
	}

	
}
