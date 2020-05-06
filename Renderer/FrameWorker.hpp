#pragma once
#include "Shared/PtrTypes.hpp"
#include "Resources/ResourceRegistryDetachedReference.hpp"
#include "Resources/HandleWrapper.hpp"
#include <vector>
#include <d3d12.h>
#include "Resources/ResourceRegistry.hpp"
#include "Resources/RenderSurface.hpp"


namespace RHA
{
	namespace DX12
	{
		class CmdList;
		class WindowSurface;
		class DepthSurface;
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
		
		//takes in commands and batches them for gpu submission. renders everything to non swap chain targets
		class FrameWorker
		{
			private: DeviceResources *resources;

			private: Queue *queue;

			private: UniquePtr<CmdAllocator> allocator;

			private: UniquePtr<CmdList> list;

			private: UniquePtr<Fence> fence;
					 					 			
			private: HANDLE event;
			
			private: std::vector<UniquePtr<class RenderCommand>> commands;

			private: ResourceRegistry *registryMaster;

			private: ResourceRegistryDetachedReference registryCopy;
			
			private: RenderSurface outputSurface;

			private: size_t commandsRecordedToList;
			
			private: static constexpr size_t recordsPerCommandList{ 50 };

			private: static constexpr size_t fenceCmdCompletionValue{ 1 };

			private: HandleWrapper globalBufferHandle;

			private: bool isAllowedToPresent;
					 
			

			public: FrameWorker
			(
				DeviceResources *resources,
				Queue *queue,
				ResourceRegistry &masterRegistry,
				const RenderSurface &outputSurface,
				HandleWrapper &&globalBuffer,
				bool shouldPresentOnComplete = true
			);

			public: FrameWorker(FrameWorker &&other) noexcept;

			public: FrameWorker &operator=(FrameWorker &&rhs) noexcept;
			
			public: FrameWorker(const FrameWorker &) = delete;

			public: FrameWorker &operator=(const FrameWorker &) = delete;
			
			public: ~FrameWorker() noexcept;

			public: void UnregisterResources();
			
				private: inline bool RegistryIsValid() const { return registryMaster != nullptr; }

				private: void UnregisterAllCommands();
			
			
			public: inline bool IsInvalid() const { return resources == nullptr; }

			public: void AddCommand(UniquePtr<RenderCommand> &&command);

			public: int ExecuteCommands();
			
				private: void RecordCommands();
			
					private: bool ListCapacityIsReached() const;

					private: void SubmitCurrentList();

					private: void ResetCurrentList();

				private: void SetupCompletionFence();
			
			public: void WaitForCompletion();

				private: void PresentIfAllowed();

													
		};
		
		
	}
	
	
}
