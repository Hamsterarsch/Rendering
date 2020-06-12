#include "AssetSystem/IO/Writer.hpp"


namespace assetSystem
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
