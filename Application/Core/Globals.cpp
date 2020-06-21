#include "Core/Globals.hpp"
#include "AssetSystem/Interface/AssetSystem.hpp"
#include "CreateProject.hpp"
#include "Shared/Filesystem/Conversions.hpp"


namespace App::Core
{
	Globals::Globals()
		:
		programVersion(0, 0, 0)		
	{
		bool hasVersionMismatch{ false };
		programAssetSystem = LoadProject(Filesystem::Conversions::MakeExeRelative(L"../../ProgramContent/ProgramContent.proj.asset").c_str(), hasVersionMismatch);
		
	}

	Globals::~Globals() = default;

	
}
