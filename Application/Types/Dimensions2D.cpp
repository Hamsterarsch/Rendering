#include "Dimensions2D.hpp"


Dimensions2D::Dimensions2D(const unsigned width, const unsigned height)
{
	this->width =  width  >= 0 ? width	: 0;
	this->height = height >= 0 ? height : 0;
	
}		



void Dimensions2D::SetWidth(const unsigned width)
{
	if(width >= 0)
	{
		this->width = width;
	}
	
}

void Dimensions2D::SetHeight(const unsigned height)
{
	if(height >= 0)
	{
		this->height = height;
	}
	
}
	
	

