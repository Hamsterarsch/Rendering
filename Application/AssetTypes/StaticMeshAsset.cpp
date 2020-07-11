#include "AssetTypes/StaticMeshAsset.hpp"
#include "IO/Archive.hpp"


namespace App::Assets
{
	assetSystem::io::Archive &StaticMeshAsset::Serialize(assetSystem::io::Archive &archive)
	{
		return archive.Serialize("staticMeshData", meshData);
				
	}

	

	void StaticMeshAsset::UploadToRenderer(Renderer::RendererFacade &renderer)
	{
		
	}



	const char *StaticMeshAsset::GetAssetClassExtension()
	{
		static const char *extension{ "msh" };

		return extension;
		
	}

	
}
