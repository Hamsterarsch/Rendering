#pragma once
#include "Shared/Types/Containers/QueueConcurrent.hpp"
#include <future>
#include "FrameRenderer.hpp"


namespace Renderer
{
	namespace DX12
	{
		class RendererMaster
		{
			private: QueueConcurrent<FrameRenderer> inputQueue;

			private: QueueConcurrent<FrameRenderer> *outputQueue;

			private: std::future<int> updaterHandle;

			private: bool shouldUpdateRendering;

			private: std::future<int> activeFrameHandle;

			private: unsigned char maxScheduledFrames;

			
			
			public: RendererMaster(QueueConcurrent<FrameRenderer> &outputQueue, unsigned char maxScheduledFrames);

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

			public: void ScheduleFrame(FrameRenderer &&frame);
			
			public: bool HasNoCapacityForFrames() const;

			
			
			
		};
		
		
	}

	
}
