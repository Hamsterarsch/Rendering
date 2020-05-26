#pragma once
#include "AssetSystem/IO/Archive.hpp"
#include "AssetSystem/Core/Assets/AssetSystemConfigAsset.hpp"
#include <filesystem>


namespace AssetSystem
{
	namespace fs = std::filesystem;
	
	class AssetRegistry : public IO::Archivable
	{
		private: fs::path configDir;

		private: Assets::AssetSystemConfigAsset config;

		
		
		public: AssetRegistry();



		IO::Archive& Serialize(IO::Archive& archive) override{ return archive; }
	};
	
	
}

