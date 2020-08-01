#include "Core/AssetHashingUtils.hpp"
#include <filesystem>
#include "AssetFileending.hpp"
#include "Shared/Hashing/CRC32.hpp"


namespace assetSystem::core
{	
	AssetKey MakeAssetKeyWithPathCleaning(const char *assetPathRelativeToRootFolder)
	{
		const auto cleanPath{ EnsureCorrectPathFormat(assetPathRelativeToRootFolder) };
		return Hashing::CRC32Mpeg2::Generate( cleanPath.c_str(), cleanPath.size());		
		
	}


	
			std::string ReplaceBackslashes(std::string &&path)
			{
				for(auto replacePos{ path.find_first_of('\\') }; replacePos != std::string::npos; replacePos = path.find_first_of('\\'))
				{
					path.at(replacePos) = '/';
				}

				return std::move(path);
			
			}
	
		std::string EnsureCorrectPathFormat(const char *path)
		{
			std::filesystem::path filepath{ path };

			if(filepath.extension() == GetAssetFileending())
			{
				filepath.replace_extension();
			}

			return ReplaceBackslashes(filepath.string());
		
		}



	AssetKey MakeAssetKey(const std::string &cleanPath)
	{
		return Hashing::CRC32Mpeg2::Generate(cleanPath.c_str(), cleanPath.size());
		
	}

	
}
