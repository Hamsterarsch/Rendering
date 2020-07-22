#pragma once
#include <forward_list>
#include "Resources/HandleFactory.hpp"
#include "Resources/HandleMap.hpp"
#include "Resources/Descriptor/ReferenceAwareDescriptorAllocator.hpp"

#include "Resources/HasQueriableResources.hpp"
#include "Resources/RootSignature/RootSignatureData.hpp"
#include "Resources/ResourceAllocation.hpp"
#include "Shared/PtrTypes.hpp"
#include "Descriptor/DescriptorAllocator.hpp"
#include "DX12/WindowSurface.hpp"


namespace Renderer
{
	class SerializationHook;
	class HandleFactory;
}


namespace Renderer::DX12
{		
	class RootSignatureFactory;


	struct RawDescriptorReference
	{
		ResourceHandle::t_hash referencedResource;
		DescriptorMemory *memorySource;
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor;

		RawDescriptorReference(DescriptorMemory &memorySource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor, ResourceHandle::t_hash referencedResource)
			:
			referencedResource{ referencedResource },
			memorySource{ &memorySource },
			descriptor{ descriptor }
		{}
		
	};



	
	inline ID3D12Resource *HandleMapResourceAccessor(ResourceAllocation &alloc) { return alloc.resource.Get(); }		
	using HandleMapResource	= HandleMap<ResourceAllocation, ID3D12Resource *, &HandleMapResourceAccessor>;

	inline ID3D12PipelineState *HandleMapPsoAccessor(DxPtr<ID3D12PipelineState> &entity) { return entity.Get(); }		
	using HandleMapPso = HandleMap<DxPtr<ID3D12PipelineState>, ID3D12PipelineState *,	&HandleMapPsoAccessor>;
					
	inline RHA::DX12::WindowSurface *HandleMapWindowSurfaceAccessor(UniquePtr<RHA::DX12::WindowSurface> &entity) { return entity.get(); }		
	using HandleMapWindowSurface = HandleMap<UniquePtr<RHA::DX12::WindowSurface>, RHA::DX12::WindowSurface *, &HandleMapWindowSurfaceAccessor>;

	
	class ResourceRegistry final : public UsesReferences, public HasQueriableResources, public CanPurgeUnreferencedEntities
	{		
		private: HandleMapResource registryResource;

		private: HandleMapPso registryPso;

		private: HandleMap<RootSignatureData> registrySignature;
				 
		private: HandleMapWindowSurface registryWindowSurface;

		private: HandleMap<ReferenceAwareDescriptorAllocator> registryDescriptor;

		private: HandleMap<RawDescriptorReference> registryRawDescriptor;
		
		private: HandleFactory handleProvider;

		private: std::forward_list<ResourceHandle::t_hash> handlesToRetire;


		
		public: ResourceRegistry();

		
		public: bool IsHandleUnknown(ResourceHandle::t_hash handle)	const;

		public: ResourceHandle::t_hash Register(ResourceAllocation &&allocation);
		
		public: ResourceHandle::t_hash Register(RootSignatureData &&signature);

		public: ResourceHandle::t_hash Register(DxPtr<ID3D12PipelineState> &&pipeline);

		public: ResourceHandle::t_hash Register(UniquePtr<RHA::DX12::WindowSurface> &&surface);

		public: ResourceHandle::t_hash Register(ReferenceAwareDescriptorAllocator &&allocator);

		public: ResourceHandle::t_hash Register(RawDescriptorReference &&descriptor);
		
		
		public: virtual ID3D12Resource *GetResource(ResourceHandle::t_hash handle) override;

		public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGpuAddress(ResourceHandle::t_hash handle) override;

		public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) override;

		public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) override;

		public: virtual RHA::DX12::WindowSurface *GetSurface(ResourceHandle::t_hash handle) override;

		public: DescriptorAllocator &GetDescriptorAllocator(ResourceHandle::t_hash handle) override;

		public: D3D12_CPU_DESCRIPTOR_HANDLE GetRawDescriptor(ResourceHandle::t_hash handle) override;

		public: bool IsWindowSurfaceReferenced(ResourceHandle::t_hash handle) const;
				
		
		public: size_t GetSignatureCbvOffset(ResourceHandle::t_hash signature, unsigned tableIndex, size_t cbvOrdinal);

			private: size_t GetSignatureOffset(ResourceHandle::t_hash signature, unsigned tableIndex, size_t ordinal, size_t (TableLayout:: *getter)(unsigned short) const);
		
		public: size_t GetSignatureSrvOffset(ResourceHandle::t_hash signature, unsigned tableIndex, size_t srvOrdinal);
		
		public: size_t GetSignatureUavOffset(ResourceHandle::t_hash signature, unsigned tableIndex, size_t uavOrdinal);


		public: void RetireHandle(ResourceHandle::t_hash handle);

		public: virtual void PurgeUnreferencedEntities() override;
				
		public: virtual void AddReference(ResourceHandle::t_hash handle) override;

		private: void ExecuteReferenceOperationOnCorrectRegistry(ResourceHandle::t_hash handle, void (UsesReferences::*operation)(ResourceHandle::t_hash));
		
		public: virtual void RemoveReference(ResourceHandle::t_hash handle) override;
		
	};

	
}

	

