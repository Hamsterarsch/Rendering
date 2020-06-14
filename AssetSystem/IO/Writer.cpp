#include "AssetSystem/IO/Writer.hpp"


namespace assetSystem
{
	bool io::Writer::IsWriting() const
	{
		return true;
	}
	
	bool io::Writer::IsReading() const
	{
		return false;
	}
	
	
}
