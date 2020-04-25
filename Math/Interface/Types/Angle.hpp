#pragma once
#include "MathExportHelper.hpp"


namespace Math
{
	class MATH_DLLSPEC Angle
	{
		private: float angle;
		
		
		public: Angle();

		public: Angle(float angle);

			private: void SetAngle(float angle);

		public: operator float() const;

		public: Angle &operator+=(const Angle &rhs);

		public: Angle &operator-=(const Angle &rhs);		
		
	};

	Angle MATH_DLLSPEC operator+(const Angle &lhs, const Angle &rhs);

	Angle MATH_DLLSPEC operator-(const Angle &lhs, const Angle &rhs);
	
	
}
