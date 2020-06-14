#include "AssetSystem/IO/Reader.hpp"


namespace assetSystem::io
{
	bool Reader::IsWriting() const
	{
		return false;
		
	}


	
	bool Reader::IsReading() const
	{
		return true;
		
	}

	
}
