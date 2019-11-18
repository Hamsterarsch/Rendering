#pragma once
#include "Shared/PtrTypes.hpp"
#include "DxPtrTypes.hpp"


namespace RHA
{
	namespace DX12
	{
		class CmdAllocator;
		class Queue;
		class DeviceResources;
	}
}

struct ID3D12Resource;

namespace Renderer
{
	namespace DX12
	{		
		class FrameRenderer
		{
			//takes in commands and batches them for gpu submission. renders everything to non swap chain targets

			private: RHA::DX12::DeviceResources *resources;

			private: RHA::DX12::Queue *queue;

			private: UniquePtr<RHA::DX12::CmdAllocator> allocator;

			private: DxPtr<ID3D12Resource> renderTarget;

			

			public: FrameRenderer(RHA::DX12::DeviceResources *resources, RHA::DX12::Queue *queue, unsigned frameWidth, unsigned frameHeight);

			public: FrameRenderer(const FrameRenderer &) = delete;

			public: FrameRenderer &operator=(const FrameRenderer &) = delete;

			public: FrameRenderer(FrameRenderer &&) noexcept = default;

			public: FrameRenderer &operator=(FrameRenderer &&) noexcept = default;
			
			public: ~FrameRenderer() noexcept;
								
		};
		
		
	}
	
	
}
