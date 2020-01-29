#pragma once
#include "DxPtrTypes.hpp"
#include "Shared/PtrTypes.hpp"
#include "DX12/DescriptorHeap.hpp"
#include "DX12/DepthSurface.hpp"

namespace RHA
{
	namespace DX12
	{
		class DeviceResources;

		class DepthSurfaceImpl : public DepthSurface
		{
			private: DxPtr<ID3D12Resource> depthTarget;

			private: UniquePtr<DescriptorHeap> viewHeap;


			public: DepthSurfaceImpl(DeviceResources *resources, D3D12_RESOURCE_DESC surfaceSpecsToMatch);

				private: static void CheckCreation(HRESULT result);
			
			public: virtual D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCpu() const override;
			
		};

		
	}
	
	
}
