#pragma once
#include <unordered_map>
#include <unordered_set>
#include "UsesReferences.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Shared/Exception/Exception.hpp"
#include <functional>
#include "Exceptions/InvalidRegistryHandleException.hpp"


namespace Renderer { struct ResourceHandle; }
	
	
namespace Renderer::DX12
{
	class CanPurgeUnreferencedEntities
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CanPurgeUnreferencedEntities)

		public: virtual void PurgeUnreferencedEntities() = 0;
		
	};

	template<class t_mapValue> t_mapValue &HandleMapAccessorDefault(t_mapValue &entity) { return entity; }
	
	template<class t_entity, class t_get = std::add_lvalue_reference_t<t_entity>, t_get (*accessor)(t_entity &) = &HandleMapAccessorDefault<t_entity>>
	class HandleMap final :  public UsesReferences, public CanPurgeUnreferencedEntities
	{						
		private: std::unordered_map<ResourceHandle::t_hash, t_entity> entities;
		
		private: std::unordered_map<ResourceHandle::t_hash, size_t> references;

		private: std::unordered_set<ResourceHandle::t_hash> unreferenced;

		private: std::function<void(t_entity &)> onEntityPurged;
				 					 			
		
		public: t_get Get(ResourceHandle::t_hash handle) 
		{
			Exception::ThrowIfDebug(handle == 0, Exceptions::InvalidRegistryHandleException{ handle, "Renderer::Registry: Tried to get a resource with a zero handle"});
			Exception::ThrowIfDebug(entities.find(handle) == entities.end(), Exceptions::InvalidRegistryHandleException{ handle, "Renderer::Registry: The specified resource is not available in this registry on get." });
			
			return accessor(entities.at(handle));
			
		}			


		
		public: void Register(ResourceHandle::t_hash handle, t_entity &&entity) 
		{
			Exception::ThrowIfDebug(entities.find(handle) != entities.end(), Exceptions::InvalidRegistryHandleException{ handle, "Renderer::Registry: Tried to register a handle that was already registered." });
			
			
			entities.insert( {handle, std::move(entity)} );
			references.insert( {handle, 1} );
			//unreferenced.emplace(handle);
			
		}


		
		public: virtual void AddReference(ResourceHandle::t_hash handle) override
		{
			Exception::ThrowIfDebug(entities.find(handle) == entities.end(), Exceptions::InvalidRegistryHandleException{ handle, "Renderer::Registry: The specified resource is not available in this registry on reference addition. Please ensure that the reference was not purged previously." });
			
			auto &counter{ references.at(handle) };

			if(counter == 0)
			{
				unreferenced.erase(handle);					
			}

			++counter;
			
		}


		
		public: virtual void RemoveReference(ResourceHandle::t_hash handle) override
		{
			Exception::ThrowIfDebug(entities.find(handle) == entities.end(), Exceptions::InvalidRegistryHandleException{ handle, "Renderer::Registry: The specified resource is not available in this registry on reference removal." });
			
			auto &counter{ references.at(handle) };

			if(counter == 1)
			{
				unreferenced.emplace(handle);
			}

			if(counter > 0)
			{
				--counter;					
			}				
			
		}

				
		
		public: virtual void PurgeUnreferencedEntities() override
		{
			for(auto &&handle : unreferenced)
			{
				if(onEntityPurged)
				{
					onEntityPurged(entities.at(handle));						
				}
				
				entities.erase(handle);					
				references.erase(handle);					
			}
			unreferenced.clear();
			
		}


		
		public: virtual bool IsHandleUnknown(ResourceHandle::t_hash handle) const 
		{
			return references.find(handle) == references.end();
			
		}

		public: void SetOnEntityPurged(decltype(onEntityPurged) callback)
		{
			onEntityPurged = std::move(callback);
			
		}

		public: size_t GetReferenceCount(ResourceHandle::t_hash handle) const
		{
			return references.at(handle);
			
		}
	
											
	};				


}
	
