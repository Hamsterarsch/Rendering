#include "AssetTypes/PsoAsset.hpp"
#include "IO/Archive.hpp"


namespace App::Assets
{
	assetSystem::io::Archive &PsoAsset::Serialize(assetSystem::io::Archive &archive)
	{
		using psoType_t = std::underlying_type_t<decltype(type)>;
		
		uint32_t typeContainer{ static_cast<psoType_t>(type) };
		archive.Serialize("pipelineType", typeContainer);
		type = static_cast<decltype(type)>(typeContainer);
				
		return archive.Serialize("pixelShader", pixelShader);
		
	}


	
	const char *PsoAsset::GetAssetClassExtension()
	{
		static const char *extension{ "pso" };

		return extension;
		
	}
	
}
