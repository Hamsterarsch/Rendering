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

			private: struct UpdaterInfo
			{
				int result;
				std::exception exception;
			};
			
			private: std::future<UpdaterInfo> updaterHandle;

			private: bool shouldProcessWorkers;
			
			private: std::future<int> activeWorkerHandle;

					
			
			public: RendererMaster(QueueConcurrent<FrameWorker> &outputQueue);

			private: UpdaterInfo Update();

					private: void ProcessNextWorker();

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
