#pragma once
#include "MathExportHelper.hpp"
#include "ThirdParty/glm/mat4x4.hpp"
#include "ThirdParty/glm/ext/matrix_transform.hpp"
#include "Vector.hpp"


namespace Math
{

	class MATH_DLLSPEC Matrix
	{
		private: glm::mat4x4 data;


		public: Matrix();
		
		private: Matrix(glm::mat4x4 &&data);
				 
		
		public: static Matrix MakeTranslation(float x, float y, float z);

		public: static Matrix MakeRotation(float pitch, float yaw, float roll);

		public: static Matrix MakeScale(float x, float y, float z);

		public: static Matrix MakeProjection(float verticalFovRadians, float screenWidth, float screenHeight, float nearDistance, float farDistance);

		public: static Matrix MakeOrthogonalProjection(float left, float screenWidth, float top, float screenHeight, float nearDistance, float farDistance);
		

		public: Matrix Inverse() const;
		
		public: Matrix &Translate(float x, float y, float z);
		
		public: Matrix &operator*=(const Matrix &other);

		public: Vector4 Transform(const Vector4 &vector) const;

						
	};

	Matrix MATH_DLLSPEC operator*(const Matrix &lhs, const Matrix &rhs);

	

		
	
}
