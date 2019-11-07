#include "Windows/App.hpp"
#include "Shared/Exception/Exception.hpp"


int main()
{
	try
	{
		Windows::App app{};				
		
	}
	catch(Exception::Exception &e)
	{
		return -1;		
	}
	catch(...)
	{		
		return -1;
	}

	return 0;
	
}
