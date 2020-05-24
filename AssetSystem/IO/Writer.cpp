#include "AssetSystem/IO/Writer.hpp"


namespace AssetSystem
{
	bool IO::Writer::IsWriting() const
	{
		return true;
	}
	
	bool IO::Writer::IsReading() const
	{
		return false;
	}
	
	
}
