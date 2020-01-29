#include <limits>
#include "Shared/Exception/Exception.hpp"
#include "Resources/SerialFactory.hpp"


namespace Renderer
{
	SerialFactory::SerialFactory() :
		currentSerial{ 0 }
	{
	}

	size_t SerialFactory::GetNextSerial()
	{
		if(currentSerial == std::numeric_limits<decltype(currentSerial)>::max())
		{
			throw Exception::Exception{ "A serial factory reached the end of its numeric range" };
		}
		
		return ++currentSerial;
		
	}

	void SerialFactory::Reset()
	{
		currentSerial = 0;
		
	}
	
	
}
