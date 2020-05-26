#pragma once
#include "AssetSystem/IO/Archive.hpp"


namespace AssetSystem::IO
{
	class Reader : public ArchiveBase
	{
		public: bool IsWriting() const final override;

		public: bool IsReading() const final override;
		
	};

	
}
