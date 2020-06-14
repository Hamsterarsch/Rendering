#include "AssetSystem.hpp"
#include "Core/AssetRegistry.hpp"
#include "Core/AssetSystemImpl.hpp"
#include "Core/AssetMemory.hpp"


namespace assetSystem
{	
	UniquePtr<AssetSystem> MakeAssetSystem(const char *projectAssetFolder)
	{			
		return MakeUnique<core::AssetSystemImpl>( core::AssetRegistry{projectAssetFolder}, core::AssetMemory{} );
		
	}

	
}
