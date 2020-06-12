#include "AssetSystem/IO/Archive.hpp"


namespace assetSystem::IO
{
	Archive &ArchiveBase::Serialize(const char *propertyName, Archivable &archivable)
	{
		EnterSubobject(propertyName);
			archivable.Serialize(*this);
		LeaveSubobject();

		return *this;
		
	}

	
}
