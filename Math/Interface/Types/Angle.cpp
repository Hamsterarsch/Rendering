#include "Types/Angle.hpp"
#include <algorithm>


namespace Math
{
	Angle::Angle() : angle{ 0 } {}


	
	Angle::Angle(const float angle)
	{
		SetAngle(angle);
		
	}

		void Angle::SetAngle(const float angle)
		{
			this->angle = std::fmod(angle, 360.f);
		
		}


			
	Angle::operator float() const
	{
		return angle;
		
	}


		
	Angle &Angle::operator+=(const Angle &rhs)
	{
		this->SetAngle(this->angle + rhs.angle);
		return *this;
		
	}
	

	
	Angle &Angle::operator-=(const Angle &rhs)
	{
		this->SetAngle(this->angle - rhs.angle);
		return *this;
		
	}





	
	Angle operator+(const Angle &lhs, const Angle &rhs)
	{
		auto temp{ lhs };
		temp += rhs;
		return temp;
		
	}


	
	Angle operator-(const Angle &lhs, const Angle &rhs)
	{
		auto temp{ lhs };
		temp -= rhs;
		return temp;
		
	}

	
}
