#pragma once
#include "IO/Archive.hpp"


namespace assetSystem::io
{
	class Writer : public ArchiveBase
	{
		public: bool IsWriting() const final override;
		
		public: bool IsReading() const final override;
		
	};
	
	
}
