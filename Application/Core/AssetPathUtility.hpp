#pragma once
#include <string>
#include "AssetFileending.hpp"


namespace App::Core
{
		inline std::string RemoveAssetFileending(const std::string &filename)
		{		
			std::string asString{ filename };
			if(asString.find(assetSystem::GetAssetFileending()) != std::string::npos)
			{
				const auto erasePos{ asString.find_last_of('.') };
				asString.erase(erasePos, asString.size()-erasePos);
			}

			return asString;
			
		}
	
	inline std::string GetAssetClassExtensionFromFilename(const std::string &filename)
	{
		auto asString{ RemoveAssetFileending(filename) };
		
		const auto extensionStartPos{ asString.find_first_of('.') };
		asString.erase(0, extensionStartPos+1);

		return asString;
		
	}

	inline std::string RemoveAllAssetExtensionsFromFilename(const std::string &filename)
	{
		std::string asString{ filename };
		
		const auto erasePos{ asString.find_first_of('.') };
		asString.erase(erasePos, asString.size()-erasePos );

		return asString;
			
	}

	
}
