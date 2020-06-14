#include "Windows/Application.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include <fstream>
#include <string>
#include <filesystem>
#include "Shared/Debugging.hpp"


int main()
{
	try
	{		
		auto &app{ App::Windows::Application::Get() };
		app.EnterLoop();
		
	}
	catch(std::exception &e)
	{
		Exception::DebugBreak();
		
		const auto directory{ Filesystem::Conversions::MakeExeRelative(L"Logs/") };
		std::filesystem::create_directory(directory);
		
		const auto seconds{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()) };
		const auto filepath{ directory+L"Log_"+std::to_wstring(seconds.count())+L".txt" };		
		std::ofstream logFile{filepath.c_str()};
				
		if(logFile.is_open())
		{
			logFile << e.what();			
		}		
		
		return -1;		
	}
	catch(...)
	{
		Exception::DebugBreak();
		return -1;
	}

	return 0;
	
}
