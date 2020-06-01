#include "Ui/ImguiTypeArithmetics.hpp"


ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs)
{
	auto out{ lhs };	
	return out += rhs;
	
}

	ImVec2 &operator+=(ImVec2 &lhs, const ImVec2 &rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
	
		return lhs;
		
	}



ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs)
{
	auto out{ lhs };	
	return out -= rhs;
	
}

	ImVec2 &operator-=(ImVec2 &lhs, const ImVec2 &rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
	
		return lhs;
		
	}



ImVec2 operator*(const ImVec2 &lhs, const ImVec2 &rhs)
{
	auto out{ lhs };	
	return out *= rhs;
	
}

	ImVec2 &operator*=(ImVec2 &lhs, const ImVec2 &rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
	
		return lhs;
		
	}



ImVec2 operator/(const ImVec2 &lhs, const ImVec2 &rhs)
{
	auto out{ lhs };	
	return out /= rhs;
	
}

	ImVec2 &operator/=(ImVec2 &lhs, const ImVec2 &rhs)
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
	
		return lhs;
		
	}



ImVec2 operator+(const ImVec2 &lhs, float scalar)
{
	auto out{ lhs };	
	return out += scalar;
	
}

	ImVec2 &operator+=(ImVec2 &lhs, float scalar)
	{
		lhs.x += scalar;
		lhs.y += scalar;
	
		return lhs;
		
	}



ImVec2 operator-(const ImVec2 &lhs, float scalar)
{
	auto out{ lhs };	
	return out -= scalar;
	
}

	ImVec2 &operator-=(ImVec2 &lhs, float scalar)
	{
		lhs.x -= scalar;
		lhs.y -= scalar;
	
		return lhs;
		
	}



ImVec2 operator*(const ImVec2 &lhs, float scalar)
{
	auto out{ lhs };	
	return out *= scalar;
	
}

	ImVec2 &operator*=(ImVec2 &lhs, float scalar)
	{
		lhs.x *= scalar;
		lhs.y *= scalar;
	
		return lhs;
		
	}



ImVec2 operator/(const ImVec2 &lhs, float scalar)
{
	auto out{ lhs };	
	return out /= scalar;
	
}

	ImVec2 &operator/=(ImVec2 &lhs, float scalar)
	{
		lhs.x /= scalar;
		lhs.y /= scalar;
	
		return lhs;
		
	}

