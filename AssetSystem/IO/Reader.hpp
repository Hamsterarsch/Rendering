#pragma once
#include "AssetSystem/IO/Archive.hpp"


namespace AssetSystem::IO
{
	class Reader : public Archive
	{
		public: bool IsWriting() const final override;

		public: bool IsReading() const final override;
		
	};

	
}
