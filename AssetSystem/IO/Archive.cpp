#include "IO/Archive.hpp"


namespace assetSystem::io
{
	Archive &ArchiveBase::Serialize(const char *propertyName, Archivable &archivable)
	{
		EnterSubobject(propertyName);
			archivable.Serialize(*this);
		LeaveSubobject();

		return *this;
		
	}

	
}
