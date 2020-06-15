#include "Resources/SerializeTarget.hpp"


namespace Renderer
{
	void SerializeTarget::Resize(const size_t sizeInBytes)
	{
		if(size < sizeInBytes)
		{
			data = MakeUnique<unsigned char[]>(sizeInBytes);				
		}
		size = sizeInBytes;
		
	}

	
}
