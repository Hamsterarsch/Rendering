#pragma once
#include "AssetSystem/IO/Archive.hpp"


namespace AssetSystem::IO
{
	class Reader : public ArchiveBase
	{
		public: bool IsWriting() const final override;

		public: bool IsReading() const final override;

		public: virtual size_t GetPropertySizeInBytes(const char *propertyName) = 0;
		
	};

	
}
