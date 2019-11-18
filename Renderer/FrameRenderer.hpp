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
		using namespace RHA::DX12;
		
		class FrameRenderer
		{
			//takes in commands and batches them for gpu submission. renders everything to non swap chain targets

			private: DeviceResources *resources;

			private: Queue *queue;

			private: UniquePtr<CmdAllocator> allocator;

			private: DxPtr<ID3D12Resource> renderTarget;

			

			public: FrameRenderer(DeviceResources *resources, Queue *queue, const DxPtr<ID3D12Resource> &renderTargetTemplate);

			public: FrameRenderer(const FrameRenderer &) = delete;

			public: FrameRenderer &operator=(const FrameRenderer &) = delete;

			public: FrameRenderer(FrameRenderer &&) noexcept = default;

			public: FrameRenderer &operator=(FrameRenderer &&) noexcept = default;
			
			public: ~FrameRenderer() noexcept;
								
		};
		
		
	}
	
	
}
