#include "Matrix.hpp"
#include "ThirdParty/glm/ext/matrix_projection.hpp"
#include "ThirdParty/glm/ext/matrix_clip_space.hpp"


namespace Math
{
	Matrix::Matrix(glm::mat4x4 && data) : data(std::move(data))
	{
	}

	Matrix Matrix::MakeTranslation(const float x, const float y, const float z)
	{			
		return Matrix{ translate(glm::identity<decltype(data)>(), {x, y, z}) };
			
	}


	
	Matrix Matrix::MakeRotation(const float pitch, const float yaw, const float roll)
	{
		Matrix out{ rotate(glm::identity<decltype(data)>(), glm::radians(pitch), {1, 0, 0}) };
		rotate( out.data, glm::radians(yaw), {0,1,0});
		rotate( out.data, glm::radians(roll), {0, 0, 1});
					
		return out;
		
	}

	Matrix Matrix::MakeProjection
	(
		const float verticalFovRadians, 
		const float screenWidth, 
		const float screenHeight, 
		const float nearDistance,
		const float farDistance
	)
	{		
		return Matrix{ glm::perspectiveFovLH_ZO(verticalFovRadians, screenWidth, screenHeight, nearDistance, farDistance) };
		
	}


	
	Matrix Matrix::MakeOrthogonalProjection(float left, float screenWidth, float top, float screenHeight, float nearDistance, float farDistance)
	{
		return Matrix{ glm::orthoLH_ZO(left, screenWidth, screenHeight, top, nearDistance, farDistance) };
		
	}


	

	Matrix Matrix::Inverse() const
	{
		return Matrix{ glm::inverse(data) };
		
	}

	

	Matrix &Matrix::Translate(const float x, const float y, const float z)
	{
		data = translate(data, {x, y, z});
		return *this;
		
	}


	Matrix &Matrix::operator*=(const Matrix &other)
	{			
		data *= other.data;
		return *this;
		
	}

	Vector4 Matrix::Transform(const Vector4& vector) const
	{
		return Vector4{ data * vector._internal };
		
	}


	Matrix operator*(const Matrix &lhs, const Matrix &rhs)
	{
		auto out{ lhs };
		out *= rhs;
		return out;

	}	

	
}
