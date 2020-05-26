#include "AssetSystem/IO/Reader.hpp"


namespace AssetSystem::IO
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
