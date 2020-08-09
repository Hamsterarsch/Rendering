#include "Windows/Application.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include <fstream>
#include <string>
#include <filesystem>
#include "Shared/Debugging.hpp"


	void LogException(const std::exception &e)
	{
		Exception::DebugBreak();
		
		const auto directory{ Filesystem::Conversions::MakeExeRelative(L"Logs/") };

		std::error_code error{};
		std::filesystem::create_directory(directory, error);
		if(error)
		{
			return;
		}
		
		const auto seconds{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()) };
		const auto filepath{ directory+L"Log_"+std::to_wstring(seconds.count())+L".txt" };		
		std::ofstream logFile{filepath.c_str()};
				
		if(logFile.is_open())
		{
			logFile << e.what();			
		}
				
	}

int main()
{
	try
	{		
		auto &app{ App::Windows::Application::Get() };
		app.EnterLoop();
		
	}
	catch(std::exception &e)
	{
		LogException(e);		
		return -1;		
	}
	catch(...)
	{
		LogException(std::exception{"Unknown error"});				
		return -1;
	}

	return 0;
	
}
