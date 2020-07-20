#include "Core/AssetSystemImpl.hpp"
#include <string>
#include "IO/Filetypes/AssetReader.hpp"
#include "Shared/Debugging.hpp"
#include "Shared/Exception/Exception.hpp"
#include "IO/Filetypes/AssetWriter.hpp"
#include "AssetFileending.hpp"
#include "AssetHashingUtils.hpp"


namespace assetSystem::core
{
	AssetSystemImpl::AssetSystemImpl(AssetRegistry &&registry, AssetMemory &&memory)
		:
		registry{ std::move(registry) },
		memory{ std::move(memory) }
	{
		if(not exists(GetReferenceDataPath()))
		{
			SaveReferenceData(true);
		}
		
		LoadReferenceData();
		
		
	}

		fs::path AssetSystemImpl::GetReferenceDataPath() const
		{
			return registry.GetAssetDirectory() / "AssetSystemReferences.ref";
	
		}
	
		void AssetSystemImpl::SaveReferenceData(bool forceRewrite)
		{
			if(serializedReferences.WasDataChanged() || forceRewrite)
			{
				io::AssetWriter writer{ GetReferenceDataPath(), {} };
				serializedReferences.Serialize(writer);				
			}
			serializedReferences.ClearChangedState();
		
		}
		
		void AssetSystemImpl::LoadReferenceData()
		{
			io::AssetReader reader{ GetReferenceDataPath(), {} };
			serializedReferences.Serialize(reader);
		
		}



	void AssetSystemImpl::RegisterAssetClass
	(
		const char *classFileExtension,
		UniquePtr<AssetConstructionOperations> &&constructOperations
	)
	{
		memory.RegisterAssetClass(classFileExtension, std::move(constructOperations));
		
	}
	

	
	AssetPtr AssetSystemImpl::MakeAsset(const char *assetPath, Asset &&assetData)
	{		
		auto cleanPath{ GetSafeAssetPathFromUnsafePath(assetPath) };		
		const auto key{ MakeAssetKey(cleanPath) };
		
		auto &asset{ ConstructAsset(key, std::move(cleanPath), std::move(assetData)) };		
		WriteAssetToFile(key, asset);
						
		return AssetPtr{ asset, key, *this };
		
	}

		std::string AssetSystemImpl::GetSafeAssetPathFromUnsafePath(const char *path) const 
		{					
			std::filesystem::path filepath{ path };
			if(filepath.is_absolute())
			{
				filepath = relative(path, registry.GetAssetDirectory());				
			}

			return EnsureCorrectPathFormat(filepath.string().c_str());
		
		}
				
		Asset &AssetSystemImpl::ConstructAsset(const AssetKey key, std::string &&projectRelativePath, Asset &&assetData)
		{
			Exception::ThrowIfDebug(registry.IsAssetRegistered(key), {"You are trying to create an asset under a path already in use by an existing asset. To serialize existing assets, please use SerializeAsset"});
						
			auto &asset{ memory.MakeAsset(std::move(assetData), key, GetAssetClassExtension(projectRelativePath.c_str()).c_str()) };
			registry.RegisterAsset(std::move(projectRelativePath));
			ptrReferences.AddReference(key);

			return asset;
		
		}
	
			std::string AssetSystemImpl::GetAssetClassExtension(const char *projectRelativePath)
			{
				const std::string relativePathAsString{ projectRelativePath };
				const auto offsetToExtension{ relativePathAsString.find_first_of('.') };
			
				return relativePathAsString.substr(offsetToExtension+1, relativePathAsString.size() - (offsetToExtension+1));
			
			}

		void AssetSystemImpl::WriteAssetToFile(const AssetKey key, Asset &asset)
		{
			const auto absoluteAssetPath{ registry.GetAbsoluteAssetPath(key) };
			create_directories(absoluteAssetPath.parent_path());
			asset.OnMakeAsset(absoluteAssetPath.string().c_str());
			
			io::AssetWriter writer{ absoluteAssetPath, [outer = key, &r = serializedReferences](AssetKey encounteredAsset)
			{
				r.AddReference(outer, encounteredAsset);
			}};		
			asset.Serialize(writer);
			SaveReferenceData();
		
		}

			
	   
	AssetPtr AssetSystemImpl::GetAsset(const char *assetPath)
	{			
		return AssetPtr(assetPath, *this);
		
	}


	
	bool AssetSystemImpl::DoesAssetExist(const char *assetPath) const
	{
		return registry.IsAssetRegistered(GetAssetKeyFromUnsafePath(assetPath));
		
	}

		AssetKey AssetSystemImpl::GetAssetKeyFromUnsafePath(const char *path) const
		{
			return MakeAssetKey(GetSafeAssetPathFromUnsafePath(path));
		
		}



	void AssetSystemImpl::SerializeAsset(AssetPtr &assetPtr)
	{
		const auto key{ assetPtr.GetCurrentKey() };
		io::AssetWriter writer{ registry.GetAbsoluteAssetPath(key), [outer = key, &r = serializedReferences ](AssetKey encounteredAsset)
		{
			r.AddReference(outer, encounteredAsset);
		}};
		
		assetPtr->Serialize(writer);
				
	}


	
	void AssetSystemImpl::ReloadAsset(AssetPtr &assetPtr)
	{
		io::AssetReader reader{ registry.GetAbsoluteAssetPath(assetPtr.GetCurrentKey()), [&asys = *this](AssetKey assetToLoad)
		{
			return asys.GetAssetFromPotentiallyRedirectedKey(assetToLoad);			
		}};

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



	bool AssetSystemImpl::TryToDeleteAsset(const char *assetPath)
	{
		const auto key{ GetAssetKeyFromUnsafePath(assetPath) };
		if(AssetCanBeDeletedWithoutReplacement(key))
		{
			registry.UnregisterAsset(key);
			return true;			
		}
		return false;		
				
	}

		bool AssetSystemImpl::AssetCanBeDeletedWithoutReplacement(AssetKey key) const
		{
			return ptrReferences.HasNoReferences(key) && serializedReferences.HasNoReferences(key);
		
		}



	void AssetSystemImpl::DeleteAsset
	(
		const char *pathToAssetToDelete,
		const char *pathToReplacementAsset
	)
	{
		const auto deleteKey{ GetAssetKeyFromUnsafePath(pathToAssetToDelete) };			   		
		const auto replacementKey{ GetAssetKeyFromUnsafePath(pathToReplacementAsset) };

		if(memory.IsAssetLoaded(deleteKey))
		{			
			memory.FreeAsset(deleteKey);			
		}
		
		if(ptrReferences.HasReferences(deleteKey) && ptrReferences.HasReferences(replacementKey))
		{
			ptrReferences.AddSourceReferenceCountToTarget(deleteKey, replacementKey);					
		}

		registry.AddAssetRedirect(deleteKey, replacementKey);
		ApplyAssetRedirectsToAssetFilesReferencing(deleteKey);		
				
	}

		void AssetSystemImpl::ApplyAssetRedirectsToAssetFilesReferencing(AssetKey source)
		{
			serializedReferences.ForEachReference(source, [&asys = *this](AssetKey referencer)
			{
				asys.GetAssetFromPotentiallyRedirectedKey(referencer).SaveToDisk();
			});
			serializedReferences.RemoveAllReferencesFor(source);
			SaveReferenceData();
		
		}

			AssetPtr AssetSystemImpl::GetAssetFromPotentiallyRedirectedKey(AssetKey key)
			{
				if(AssetWasInvalidated(key))
				{
					key = registry.GetAssetRedirect(key);
				}
				
				return GetAsset(registry.GetAbsoluteAssetPath(key).string().c_str());
				
			}


	
	void AssetSystemImpl::MoveAsset(const char *sourceAssetPath, const char *targetAssetPath)
	{		
		const auto targetKey{ GetAssetKeyFromUnsafePath(targetAssetPath) };
		if(registry.IsAssetRegistered(targetKey))
		{
			throw Exception::Exception{ "AssetSystem: Tried to move to a path that was already occupied" };
		}

		const auto sourceKey{ GetAssetKeyFromUnsafePath(sourceAssetPath) };
		RedirectAsset(sourceKey, targetKey);
		registry.RegisterAsset(GetSafeAssetPathFromUnsafePath(targetAssetPath));
		
	}

		void AssetSystemImpl::RedirectAsset(AssetKey source, AssetKey target)
		{
			if(memory.IsAssetLoaded(source))
			{			
				memory.RenameAsset(source, target);
				ptrReferences.AddSourceReferenceCountToTarget(source, target);
			}

			registry.AddAssetRedirect(source, target);
			ApplyAssetRedirectsToAssetFilesReferencing(source);			
		
		}


		
	bool AssetSystemImpl::AssetWasInvalidated(const AssetKey key) const
	{
		return registry.HasAssetRedirect(key);
		
	}


	
	LoadedAssetInfo AssetSystemImpl::GetUpdatedAssetData(const AssetKey oldKey)
	{
		ptrReferences.RemoveReference(oldKey);
		if(ptrReferences.HasNoReferences(oldKey))
		{
			registry.UnregisterAsset(oldKey);
		}		
				
		const auto newKey{ registry.GetAssetRedirect(oldKey) };
				
		LoadedAssetInfo out;
		out.key = newKey;
		out.asset = &memory.GetAsset(newKey);

		return out;
		
	}

	

	LoadedAssetInfo AssetSystemImpl::GetAssetInternal(const char *path)
	{
		const auto safePath{ GetSafeAssetPathFromUnsafePath(path) };		
		LoadedAssetInfo info{ nullptr, MakeAssetKey(safePath) };

		
		if(memory.IsAssetLoaded(info.key))
		{
			info.asset = &memory.GetAsset(info.key);
			ptrReferences.AddReference(info.key);
			return info;
		}
		
		if(not registry.IsAssetRegistered(info.key))
		{
			Exception::DebugBreak();
			Exception::ThrowIfDebug
			({
				"You are trying to load an asset that is not registered with the asset registry.\
				This either means that there is no asset under the specified path or that an external source added the asset file after asset system construction."
			});
		}
					
		info.asset = &memory.MakeAsset(info.key, GetAssetClassExtension(safePath.c_str()).c_str());
		ptrReferences.AddReference(info.key);		
		WriteFileToAsset(info.key, *info.asset);
		
		return info;
		
	}

		void AssetSystemImpl::WriteFileToAsset(const AssetKey key, Asset &asset) 
		{
			const auto filePath{ registry.GetAbsoluteAssetPath(key) };
			io::AssetReader reader{ filePath, [&asys = *this](AssetKey assetToLoad)
			{
				return asys.GetAssetFromPotentiallyRedirectedKey(assetToLoad);			
			}};
		
			asset.Serialize(reader);
			asset.OnAssetLoaded(filePath.string().c_str());			
		
		}



	void AssetSystemImpl::RemoveReference(const AssetKey key)
	{
		ptrReferences.RemoveReference(key);

		if(ptrReferences.HasReferences(key))
		{
			memory.FreeAsset(key);
		}
		
	}



	void AssetSystemImpl::AddReference(const AssetKey key)
	{
		Exception::ThrowIfDebug(registry.HasAssetRedirect(key), {"AssetSystem: It is forbidden to increase the reference count of an asset that was redirected"});
		
		ptrReferences.AddReference(key);
		
	}

	
}
