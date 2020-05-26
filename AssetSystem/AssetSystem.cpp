#include "AssetSystem.hpp"


#include "AssetSystem/IO/Filetypes/Test/AssetArchiveTests.hpp"
#include "IO/Filetypes/AssetReader.hpp"
#include "Core/AssetRegistry.hpp"


namespace AssetSystem
{
	
	UniquePtr<AssetSystem> GetAssetSystem()
	{
		::AssetSystem::IO::AssetArchiveTests::Test();
		AssetRegistry reg{};
		
		return {};
	}

	

	
}
