#include "Core/AssetSystemImpl.hpp"
#include <string>
#include "IO/Filetypes/AssetReader.hpp"
#include "Shared/Debugging.hpp"
#include "Shared/Exception/Exception.hpp"
#include "IO/Filetypes/AssetWriter.hpp"
#include "AssetFileending.hpp"


namespace assetSystem::core
{
	AssetSystemImpl::AssetSystemImpl(AssetRegistry &&registry, AssetMemory &&memory)
		:
		registry{ std::move(registry) },
		memory{ std::move(memory) }
	{}


	
	void AssetSystemImpl::RegisterAssetClass
	(
		const char *classFileExtension,
		UniquePtr<AssetConstructionOperations> &&constructOperations
	)
	{
		memory.RegisterAssetClass(classFileExtension, std::move(constructOperations));
		
	}
	

	
	AssetPtr AssetSystemImpl::MakeAsset(const char *path, Asset &&assetData)
	{		
		auto projectRelativePath{ EnsureProjectRelativePath(path) };		
		const auto key{ AssetRegistry::MakeAssetKey(projectRelativePath.c_str()) };
		
		Exception::ThrowIfDebug(registry.IsAssetKnown(key), {"You are trying to create an asset under a path already in use by an existing asset. To serialize existing assets, please use SerializeAsset"});

		auto &asset{ memory.MakeAsset(std::move(assetData), key, GetAssetClassExtension(path).c_str()) };
		registry.RegisterAsset(std::move(projectRelativePath));
		
		const auto absoluteAssetPath{ registry.GetAbsoluteAssetPath(key) };
		create_directories(absoluteAssetPath.parent_path());
		asset.OnMakeAsset(absoluteAssetPath.string().c_str());
		
		io::AssetWriter writer{ absoluteAssetPath };

		asset.Serialize(writer);

		return AssetPtr{ asset, key, *this };
		
	}

		std::string AssetSystemImpl::EnsureProjectRelativePath(const char *path) const 
		{					
			std::filesystem::path filepath{ path };
			if(filepath.is_absolute())
			{
				filepath = relative(path, registry.GetAssetDirectory());				
			}

			if(filepath.extension() == GetAssetFileending())
			{
				filepath.replace_extension();
			}

			auto stringPath{  filepath.string() };
		
			for(auto pos{ stringPath.find_first_of('\\') }; pos != stringPath.npos; pos = stringPath.find_first_of('\\'))
			{
				stringPath.at(pos) = '/';				
			}
					
			return stringPath;
		
		}

		std::string AssetSystemImpl::GetAssetClassExtension(const char *projectRelativePath)
		{
			const std::string relativePathAsString{ projectRelativePath };
			const auto offsetToExtension{ relativePathAsString.find_first_of('.') };
		
			return relativePathAsString.substr(offsetToExtension+1, relativePathAsString.size() - (offsetToExtension+1));
		
		}



	AssetPtr AssetSystemImpl::GetAsset(const char *projectRelativePath)
	{			
		return AssetPtr(projectRelativePath, *this);
		
	}


	
	bool AssetSystemImpl::DoesAssetExist(const char *projectRelativePath) const
	{
		return registry.IsAssetKnown(AssetRegistry::MakeAssetKey(projectRelativePath));
		
	}



	void AssetSystemImpl::SerializeAsset(AssetPtr &assetPtr)
	{
		io::AssetWriter writer{ registry.GetAbsoluteAssetPath(assetPtr.GetKey()) };
		
		assetPtr->Serialize(writer);
				
	}


	
	void AssetSystemImpl::ReloadAsset(AssetPtr &assetPtr)
	{
		io::AssetWriter reader{ registry.GetAbsoluteAssetPath(assetPtr.GetKey()) };

		assetPtr->Serialize(reader);
		
	}


	
	bool AssetSystemImpl::IsSameRootAssetPath(const char *asThisPath) const
	{		
		return registry.GetAssetDirectory() == fs::path{ asThisPath };
		
	}


	
	std::string AssetSystemImpl::GetRootAssetPath() const
	{
		return registry.GetAssetDirectory().string();
		
	}



	LoadedAssetInfo AssetSystemImpl::GetAssetInternal(const char *path)
	{
		const auto projectRelativePath{ EnsureProjectRelativePath(path) };
		
		LoadedAssetInfo info{ nullptr, AssetRegistry::MakeAssetKey(projectRelativePath) };
		
		if(memory.IsAssetLoaded(info.key))
		{
			info.asset = &memory.GetAsset(info.key);			
		}
		else
		{
			if(registry.IsAssetKnown(info.key))
			{				
				info.asset = &memory.MakeAsset(info.key, GetAssetClassExtension(projectRelativePath.c_str()).c_str());

				const auto filePath{ registry.GetAbsoluteAssetPath(info.key) };
				io::AssetReader reader{ filePath };
				info.asset->Serialize(reader);
				info.asset->OnAssetLoaded(filePath.string().c_str());
			}
			else
			{
				Exception::DebugBreak();
				Exception::ThrowIfDebug
				({
					"You are trying to load an asset that is not registered with the asset registry.\
					This either means that there is no asset under the specified path or that an external source added the asset file after asset system construction."
				});				
			}			
		}
		registry.AddReference(info.key);

		return info;
		
	}



	void AssetSystemImpl::RemoveReference(const AssetKey key)
	{
		registry.RemoveReference(key);

		if(registry.IsUnreferenced(key))
		{
			memory.FreeAsset(key);
		}
		
	}



	void AssetSystemImpl::AddReference(const AssetKey key)
	{
		registry.AddReference(key);
		
	}

	
}
