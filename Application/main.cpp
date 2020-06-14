#include "Windows/Application.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include <fstream>
#include <string>
#include <filesystem>

#include "AssetSystem.hpp"
#include "IO/Archive.hpp"
#include "AssetConstructOperationsHelper.hpp"

class TestAsset final : public assetSystem::Asset
{
	public: int niceData{ 0 };
	public: int niceData2{ 0 };
	public: float floatyData{ 0 };
	
	public:	assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override
	{
		return archive
		.Serialize("niceData1", niceData)
		.Serialize("niceData2", niceData2)
		.Serialize("floatyData", floatyData);
				
	}
	
};



int main()
{
	try
	{
		auto aSys{ assetSystem::MakeAssetSystem("D:/test") };
		aSys->RegisterAssetClass("test", MakeUnique<assetSystem::AssetConstructOperationsHelper<TestAsset>>());

		TestAsset t;
		t.niceData = 1;
		t.niceData2 = 2;
		t.floatyData = 0.4;

		const char *assetPath{ "testAsset.test"};
		if(aSys->DoesAssetExist(assetPath))
		{
			auto ap = aSys->GetAsset(assetPath);
			auto ep = ap;
			auto cp = std::move(ap);
		}
		else
		{
			auto ap = aSys->MakeAsset(assetPath, std::move(t));			
		}
		
		
		
		auto &app{ App::Windows::Application::Get() };
		app.EnterLoop();
		
	}
	catch(std::exception &e)
	{				
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
		return -1;
	}

	return 0;
	
}
