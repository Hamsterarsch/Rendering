#pragma once
#include "AssetSystemExportHelper.hpp"
#include "Shared/InterfaceHelpers.hpp"


namespace assetSystem
{
	namespace io { class Archive; }
	
	class ASSET_SYSTEM_DLLSPEC Archivable
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Archivable)
					
		public: virtual io::Archive &Serialize(io::Archive &archive) = 0;
				
	};

	
}
