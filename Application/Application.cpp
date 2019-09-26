#include "pch.h"
#include "Window/Windows/Window.hpp"
#include "Types/Dimensions2D.hpp"

int main()
{
	try
	{
		Windows::Window window{ {1920, 1080}, L"Window", L"UniqueClassName" };
		
	}
	catch(...)
	{		
		return -1;
	}

	return 0;
	
}
