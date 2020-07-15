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
		if(not bufferHandle)
		{
			//todo: implement resource factory multi data source support
			size_t bufferSize{ meshData.GetVertexDataSizeInBytes() + meshData.GetIndexDataSizeInBytes() };
			const auto buffer{ MakeUnique<unsigned char[]>(bufferSize) };
					
			bufferHandle = { &renderer, renderer.MakeBuffer(buffer.get(), bufferSize) };
		}
		
	}

	
	
	const char *StaticMeshAsset::GetAssetClassExtension()
	{
		static const char *extension{ "msh" };

		return extension;
		
	}

	
}
