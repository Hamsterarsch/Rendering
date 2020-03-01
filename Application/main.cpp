#include "Windows/App.hpp"
#include "Shared/Exception/Exception.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include <fstream>
#include <string>


int main()
{
	try
	{
		Windows::App app{};				
		
	}
	catch(Exception::Exception &e)
	{
		auto seconds{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()) };
				
		std::fstream logFile{Filesystem::Conversions::MakeExeRelative((L"Logs/Log_" + std::to_wstring(seconds.count())).data()), std::ios_base::out};
		logFile << e.what();
		logFile.close();
		
		return -1;		
	}
	catch(...)
	{		
		return -1;
	}

	return 0;
	
}
