#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "AssetSystemExportHelper.hpp"
#include "Shared/PtrTypes.hpp"


namespace AssetSystem
{
	class ASSET_SYSTEM_DLLSPEC AssetSystem
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(AssetSystem)
		
	};

	UniquePtr<AssetSystem> ASSET_SYSTEM_DLLSPEC GetAssetSystem();
	
}
