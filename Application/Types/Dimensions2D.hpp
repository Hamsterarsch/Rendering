#pragma once


class Dimensions2D
{	
	private: unsigned width;
	private: unsigned height;

	

	public: Dimensions2D(unsigned int width, unsigned int height);
	
	public: unsigned GetWidth() const { return width; }
	public: unsigned GetHeight() const { return height; }
	
	public: void SetWidth(unsigned width);		
	public: void SetHeight(unsigned height);

	
};

