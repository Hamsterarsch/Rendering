#pragma once
#include "Shared/PtrTypes.hpp"
#include "DxPtrTypes.hpp"
#include "RenderCommand.hpp"


namespace RHA
{
	namespace DX12
	{
		class DescriptorHeap;
		class Fence;
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

			private: UniquePtr<Fence> fence;

			private: HANDLE event;
			
			private: DxPtr<ID3D12Resource> renderTarget;

			private: DxPtr<ID3D12Resource> depthTarget;

			private: UniquePtr<DescriptorHeap> rtvHeap, dsvHeap;

			private: std::vector<UniquePtr<RenderCommand>> commands;

			private: std::vector<UniquePtr<void>> persistentCommandData;
			
			private: ResourceRegistry &registry;

			private: static constexpr size_t recordsPerCommandList{ 50 };

			private: static constexpr float clearColor[]{0,0,0,1};
			

			public: FrameRenderer(DeviceResources *resources, Queue *queue, ResourceRegistry &registry, const DxPtr<ID3D12Resource> &renderTargetTemplate);

			public: FrameRenderer(const FrameRenderer &) = delete;

			public: FrameRenderer &operator=(const FrameRenderer &) = delete;

			public: FrameRenderer(FrameRenderer &&) noexcept = default;

			public: FrameRenderer &operator=(FrameRenderer &&) noexcept = default;
			
			public: ~FrameRenderer() noexcept;


			public: void AddCommand(UniquePtr<RenderCommand> &&command);

			public: void ExecuteCommands();
			
			public: void WaitForCompletion();
			
			public: void Reinitialize();

								
		};
		
		
	}
	
	
}
