#pragma once
#include "AssetSystem/IO/Archive.hpp"


namespace assetSystem::IO
{
	class Writer : public ArchiveBase
	{
		public: bool IsWriting() const final override;
		
		public: bool IsReading() const final override;
		
	};
	
	
}
