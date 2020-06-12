#pragma once
#include "AssetSystemExportHelper.hpp"
#include "LimitedReflection.hpp"
#include <filesystem>


namespace assetSystem
{
	class ASSET_SYSTEM_DLLSPEC Asset : Reflection::LimitedReflected
	{
		DEFINE_TYPE_INFO(Asset, LimitedReflected)

		public: virtual std::filesystem::path GetSerializeSubfolder() const = 0;
					
	};
	
}
