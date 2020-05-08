#include "Utility.hpp"
#include "ThirdParty/glm/glm.hpp"


namespace Math
{

	float Radians(const float angle)
	{
		return glm::radians(angle);
		
	}


	
	float Tan(const float radians)
	{
		return std::tan(radians);
		
	}

	

	float Abs(const float number)
	{
		return std::abs(number);
		
	}

	float Log(const float number)
	{
		return std::log(number);
		
	}

	
}
