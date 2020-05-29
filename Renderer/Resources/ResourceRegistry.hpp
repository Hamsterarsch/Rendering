#pragma once
#include <forward_list>
#include "Resources/HandleFactory.hpp"
#include "Resources/Registry.hpp"
#include "Lighting/LightContainer.hpp"
#include "Resources/Descriptor/ReferenceAwareDescriptorAllocator.hpp"


namespace Renderer
{
	class SerializationHook;
	class HandleFactory;
		
	namespace DX12
	{		
		class RootSignatureFactory;

		class ResourceRegistry : public UsesReferences, public HasQueriableResources, public CanPurgeUnreferencedEntities
		{
			private: HandleMapResource registryResource;

			private: HandleMapPso registryPso;

			private: HandleMapSignature registrySignature;

			private: LightContainer registryLight;

			private: HandleMapWindowSurface registryWindowSurface;

			private: HandleMapDescriptor registryDescriptor;
			
			private: HandleFactory handleProvider;

			private: std::forward_list<ResourceHandle::t_hash> handlesToRetire;

			private: bool shouldPurgePsoAndSignature;

			
			public: explicit ResourceRegistry(bool neverPurgePsoAndSignature = true);

			public: bool IsHandleUnknown(ResourceHandle::t_hash handle)	const;

			public: ResourceHandle::t_hash Register(ResourceAllocation &&allocation);
			
			public: void Register(ResourceHandle::t_hash handle, ResourceAllocation &&allocation);

			public: ResourceHandle::t_hash Register(RootSignatureData &&signature);
			
			public: void Register(ResourceHandle::t_hash handle, RootSignatureData &&signature);

			public: ResourceHandle::t_hash Register(DxPtr<ID3D12PipelineState> &&pipeline);
			
			public: void Register(ResourceHandle::t_hash handle, DxPtr<ID3D12PipelineState> &&pipeline);

			public: ResourceHandle::t_hash Register(Light &&info);

			public: ResourceHandle::t_hash Register(UniquePtr<RHA::DX12::WindowSurface> &&surface);

			public: ResourceHandle::t_hash Register(ReferenceAwareDescriptorAllocator &&allocator);
					
			public: virtual ID3D12Resource *GetResource(ResourceHandle::t_hash handle) override;

			public: virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGpuAddress(ResourceHandle::t_hash handle) override;

			public: virtual ID3D12PipelineState *GetPso(ResourceHandle::t_hash handle) override;

			public: virtual ID3D12RootSignature *GetSignature(ResourceHandle::t_hash handle) override;

			public: virtual RHA::DX12::WindowSurface *GetSurface(ResourceHandle::t_hash handle) override;

			public: DescriptorAllocator &GetDescriptorAllocator(ResourceHandle::t_hash handle) override;

			public: Light &GetLight(ResourceHandle::t_hash handle);
					
			
			public: size_t GetSignatureCbvOffset(ResourceHandle::t_hash handle, size_t cbvOrdinal);

				private: size_t GetSignatureOffset(ResourceHandle::t_hash handle, size_t ordinal, size_t (TableLayout:: *getter)(unsigned short) const);
			
			public: size_t GetSignatureSrvOffset(ResourceHandle::t_hash handle, size_t srvOrdinal);
			
			public: size_t GetSignatureUavOffset(ResourceHandle::t_hash handle, size_t uavOrdinal);


			public: void RetireHandle(ResourceHandle::t_hash handle);

			public: virtual void PurgeUnreferencedEntities() override;
					
			public: virtual void AddReference(ResourceHandle::t_hash handle) override;

			private: void ExecuteReferenceOperationOnCorrectRegistry(ResourceHandle::t_hash handle, void (UsesReferences::*operation)(ResourceHandle::t_hash));
			
			public: virtual void RemoveReference(ResourceHandle::t_hash handle) override;


			public: const void *GetLightsData() const;

			public: size_t GetLigthsDataSizeInBytes() const;

			public: size_t GetLightCount() const;
			
		};

		
	}

	
}
