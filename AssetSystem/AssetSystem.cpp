#include "AssetSystem.hpp"


#include "AssetSystem/IO/Filetypes/Test/AssetArchiveTests.hpp"
#include "IO/Filetypes/AssetReader.hpp"


namespace AssetSystem
{
	
	UniquePtr<AssetSystem> GetAssetSystem()
	{
		::AssetSystem::IO::AssetArchiveTests::Test();
		
		return {};
	}

	

	
}
