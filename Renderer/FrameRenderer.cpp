#include "FrameRenderer.hpp"
#include <d3d12.h>
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
#include "DX12/DeviceResources.hpp"
#include "DX12/Facade.hpp"


namespace Renderer
{
	namespace DX12
	{
		FrameRenderer::FrameRenderer
		(
			DeviceResources *resources, 
			Queue *queue,
			const DxPtr<ID3D12Resource> &renderTargetTemplate
		) :
			resources{ resources },
			queue{ queue }
		{
			allocator = Facade::MakeCmdAllocator(resources, D3D12_COMMAND_LIST_TYPE_DIRECT);

			
			auto desc{ renderTargetTemplate->GetDesc() };			
			
			D3D12_HEAP_PROPERTIES properties{};
			properties.Type =  D3D12_HEAP_TYPE_DEFAULT;
			properties.CPUPageProperty =  D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			properties.CreationNodeMask =  D3D12_MEMORY_POOL_UNKNOWN;
						
			D3D12_CLEAR_VALUE clear{};
			clear.Format = desc.Format;
			clear.Color[0] = 0;
			clear.Color[1] = 0;
			clear.Color[2] = 0;
			clear.Color[3] = 1;

			auto r =
			resources->GetDevice()->CreateCommittedResource
			(
				&properties,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				&clear,
				IID_PPV_ARGS(&renderTarget)
			);



			
		}

		FrameRenderer::~FrameRenderer() noexcept = default;
			
				
	}
	
	
}
