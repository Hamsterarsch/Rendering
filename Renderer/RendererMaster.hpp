#pragma once
#include "Shared/Types/Containers/QueueConcurrent.hpp"
#include <future>
#include "FrameWorker.hpp"
#include <condition_variable>
#include <mutex>

namespace Renderer
{
	namespace DX12
	{
		class RendererMaster
		{
			private: QueueConcurrent<FrameWorker> inputQueue;

			private: QueueConcurrent<FrameWorker> *outputQueue;

			private: std::future<int> updaterHandle;

			private: bool shouldUpdateRendering;

			private: std::future<int> activeFrameHandle;

			private: bool becameIdle;

			private: std::condition_variable idleConditionVariable;

			private: std::mutex idleMutex;

					
			
			public: RendererMaster(QueueConcurrent<FrameWorker> &outputQueue);

				private: int Update();

					private: void Idle();
			
					private: void ExecuteNextFrame();

			public: RendererMaster(const RendererMaster &) = delete;

			public: RendererMaster &operator=(const RendererMaster &) = delete;

			public: RendererMaster(RendererMaster &&other) = delete;

			public: RendererMaster &operator=(RendererMaster &&rhs) = delete;

			public: ~RendererMaster() noexcept;


			public: void PropagateExceptions();

				private: bool UpdaterThreadHasFinished() const;

			public: void ScheduleFrameWorker(FrameWorker &&frame);
			
			public: size_t GetScheduledWorkerCount() const;

			public: void WaitForIdle();

			
			
			
		};
		
		
	}

	
}
