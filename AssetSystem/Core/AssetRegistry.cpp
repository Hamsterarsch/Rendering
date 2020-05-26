#include "AssetSystem/Core/AssetRegistry.hpp"
#include "Shared/Filesystem/Conversions.hpp"


namespace AssetSystem
{
	AssetRegistry::AssetRegistry() :
		configDir{ Filesystem::Conversions::MakeExeRelative("../../Config/") },
		config{ configDir }
	{		
	}
	
	
}
