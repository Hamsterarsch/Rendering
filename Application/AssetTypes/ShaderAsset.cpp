#include "AssetTypes/ShaderAsset.hpp"
#include "AssetSystem/Interface/IO/Archive.hpp"


namespace App::Assets
{
	assetSystem::io::Archive &ConstantsResourceSlot::Serialize(assetSystem::io::Archive &archive)
	{		
		return archive
		.Serialize("register", shaderRegister)
		.Serialize("size", sizeInVector4);
		
	}




	assetSystem::io::Archive &TextureResourceSlot::Serialize(assetSystem::io::Archive &archive)
	{
		return archive
		.Serialize("register", shaderRegister);
		
	}


	
	
	assetSystem::io::Archive &ShaderAsset::Serialize(assetSystem::io::Archive &archive)
	{
		if(archive.IsReading())
		{
			shaderCode.resize(archive.GetPropertySizeInBytes("code"));
		}				
		archive.Serialize("code", shaderCode.data());

		SerializeResourceSlots(archive);
		
		return archive;
		
	}

		void ShaderAsset::SerializeResourceSlots(assetSystem::io::Archive& archive)
		{
			auto numCbvSlots{ static_cast<unsigned>(constantsSlots.size()) };
			archive.Serialize("numConstantsSlots", numCbvSlots);
	
			auto numTexSlots{ static_cast<unsigned>(textureSlots.size()) };
			archive.Serialize("numTextureSlots", numTexSlots);
						
			if(archive.IsReading())
			{
				constantsSlots.resize(numCbvSlots);
				textureSlots.resize(numTexSlots);
			}


			for(unsigned slotIndex{ 0 }; slotIndex < numCbvSlots; ++slotIndex)
			{
				archive.Serialize(("cbvSlot"+std::to_string(slotIndex)).c_str(), constantsSlots.at(slotIndex));
			}

			for(unsigned slotIndex{ 0 }; slotIndex < numTexSlots; ++slotIndex)
			{
				archive.Serialize(("texSlot"+std::to_string(slotIndex)).c_str(), textureSlots.at(slotIndex));
			}
		
		}



	const char *ShaderAsset::GetAssetClassExtension()
	{
		static const char *extension{ "shdr" };

		return extension;
		
	}


	
	const wchar_t *ShaderAsset::GetAssetClassExtensionW()
	{
		static const wchar_t *extension{ L"shdr" };

		return extension;
		
	}

	
}
