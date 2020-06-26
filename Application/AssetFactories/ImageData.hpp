#pragma once
#include "Shared/PtrTypes.hpp"


namespace App::Assets
{
	struct ImageData
	{
		unsigned width;
		
		unsigned height;
		
		UniquePtr<unsigned char[]> rgbaData;
		
		size_t sizeInBytes;
				
	};

	
}
