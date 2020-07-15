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
		auto cleanPath{ EnsureCorrectPathFormat(path) };		
		const auto key{ AssetRegistry::MakeAssetKey(cleanPath.c_str()) };
		
		auto &asset{ ConstructAsset(key, std::move(cleanPath), std::move(assetData)) };		
		WriteAssetToFile(key, asset);
						
		return AssetPtr{ asset, key, *this };
		
	}

		std::string AssetSystemImpl::EnsureCorrectPathFormat(const char *path) const 
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

			return ReplaceBackslashes(filepath.string());
		
		}

			std::string AssetSystemImpl::ReplaceBackslashes(std::string &&path)
			{
				for(auto pos{ path.find_first_of('\\') }; pos != std::string::npos; pos = path.find_first_of('\\'))
				{
					path.at(pos) = '/';				
				}

				return std::move(path);
		
			}
				
		Asset &AssetSystemImpl::ConstructAsset(const AssetKey key, std::string &&projectRelativePath, Asset &&assetData)
		{
			Exception::ThrowIfDebug(registry.IsAssetKnown(key), {"You are trying to create an asset under a path already in use by an existing asset. To serialize existing assets, please use SerializeAsset"});
						
			auto &asset{ memory.MakeAsset(std::move(assetData), key, GetAssetClassExtension(projectRelativePath.c_str()).c_str()) };
			registry.RegisterAsset(std::move(projectRelativePath));
			registry.AddReference(key);

			return asset;
		
		}
	
			std::string AssetSystemImpl::GetAssetClassExtension(const char *projectRelativePath)
			{
				const std::string relativePathAsString{ projectRelativePath };
				const auto offsetToExtension{ relativePathAsString.find_first_of('.') };
			
				return relativePathAsString.substr(offsetToExtension+1, relativePathAsString.size() - (offsetToExtension+1));
			
			}

		void AssetSystemImpl::WriteAssetToFile(const AssetKey key, Asset &asset) const
		{
			const auto absoluteAssetPath{ registry.GetAbsoluteAssetPath(key) };
			create_directories(absoluteAssetPath.parent_path());
			asset.OnMakeAsset(absoluteAssetPath.string().c_str());
			
			io::AssetWriter writer{ absoluteAssetPath };
			asset.Serialize(writer);
		
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


	
	std::string AssetSystemImpl::GetAbsoluteRootAssetPath() const
	{
		return registry.GetAssetDirectory().string();
		
	}



	LoadedAssetInfo AssetSystemImpl::GetAssetInternal(const char *path)
	{
		const auto projectRelativePath{ EnsureCorrectPathFormat(path) };
		
		LoadedAssetInfo info{ nullptr, AssetRegistry::MakeAssetKey(projectRelativePath) };
		
		if(memory.IsAssetLoaded(info.key))
		{
			info.asset = &memory.GetAsset(info.key);
			registry.AddReference(info.key);
			return info;
		}
		
		if(not registry.IsAssetKnown(info.key))
		{
			Exception::DebugBreak();
			Exception::ThrowIfDebug
			({
				"You are trying to load an asset that is not registered with the asset registry.\
				This either means that there is no asset under the specified path or that an external source added the asset file after asset system construction."
			});
		}
					
		info.asset = &memory.MakeAsset(info.key, GetAssetClassExtension(projectRelativePath.c_str()).c_str());
		registry.AddReference(info.key);		
		WriteFileToAsset(info.key, *info.asset);
		
		return info;
		
	}

		void AssetSystemImpl::WriteFileToAsset(const AssetKey key, Asset &asset) const
		{
			const auto filePath{ registry.GetAbsoluteAssetPath(key) };
			io::AssetReader reader{ filePath };
			asset.Serialize(reader);
			asset.OnAssetLoaded(filePath.string().c_str());			
		
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
