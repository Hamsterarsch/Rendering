#include "Core/Version.hpp"
#include "AssetSystem/Interface/IO/Archive.hpp"


namespace App::Core
{
	assetSystem::io::Archive &Version::Serialize(assetSystem::io::Archive &archive)
	{
		return archive
		.Serialize("major", major)
		.Serialize("minor", minor)
		.Serialize("patch", patch);
		
	}

	
}
