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
			//todo: implement renderer resource creation using multiple data pointers
			size_t bufferSize{ meshData.GetVertexDataSizeInBytes() + meshData.GetIndexDataSizeInBytes() };
			const auto buffer{ MakeUnique<unsigned char[]>(bufferSize) };
			std::memcpy(buffer.get(), meshData.GetVertexData(), meshData.GetVertexDataSizeInBytes());
			std::memcpy(buffer.get() + meshData.GetVertexDataSizeInBytes(), meshData.GetIndexData(), meshData.GetIndexDataSizeInBytes());
			
			bufferHandle = { &renderer, renderer.MakeBuffer(buffer.get(), bufferSize) };
		}
		
	}

	
	
	const char *StaticMeshAsset::GetAssetClassExtension()
	{
		static const char *extension{ "msh" };

		return extension;
		
	}

	
}
