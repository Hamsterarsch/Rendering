#include "DX12/Facade.hpp"
#include "DX12/DepthSurfaceImpl.hpp"
#include "Shared/Exception/CreationFailedException.hpp"


namespace RHA
{
	namespace DX12
	{
		DepthSurfaceImpl::DepthSurfaceImpl(DeviceResources *resources, D3D12_RESOURCE_DESC surfaceSpecsToMatch)
		{
			D3D12_HEAP_PROPERTIES properties{};
			properties.Type =  D3D12_HEAP_TYPE_DEFAULT;
			properties.CPUPageProperty =  D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			properties.CreationNodeMask =  D3D12_MEMORY_POOL_UNKNOWN;			
			
			surfaceSpecsToMatch.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			surfaceSpecsToMatch.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			
			D3D12_CLEAR_VALUE clear{};
			clear.Format = surfaceSpecsToMatch.Format;
			clear.DepthStencil.Depth = 1;
			clear.DepthStencil.Stencil = 0;

			const auto result
			{
				resources->GetDevice()->CreateCommittedResource
				(
					&properties,
					D3D12_HEAP_FLAG_NONE,
					&surfaceSpecsToMatch,
					D3D12_RESOURCE_STATE_DEPTH_WRITE,
					&clear,
					IID_PPV_ARGS(&depthTarget)
				)
			};

			CheckCreation(result);

			viewHeap = Facade::MakeDescriptorHeap(resources, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);			
			resources->GetDevice()->CreateDepthStencilView(depthTarget.Get(), nullptr, viewHeap->GetHandleCpu(0));
						
		}

			void DepthSurfaceImpl::CheckCreation(const HRESULT result)
			{
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create dx12 depth surface" };
				}
			
			}
		


		D3D12_CPU_DESCRIPTOR_HANDLE DepthSurfaceImpl::GetHandleCpu() const
		{
			return viewHeap->GetHandleCpu(0);
			
		}

		
	}

	
}
