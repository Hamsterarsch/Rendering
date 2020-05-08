#include "Renderer/RendererMaster.hpp"


namespace Renderer
{
	namespace DX12
	{
		RendererMaster::RendererMaster(QueueConcurrent<FrameWorker> &outputQueue) :
			outputQueue{ &outputQueue },
			shouldUpdateRendering{ true },
			becameIdle{ false }
		{
			updaterHandle = std::async(std::launch::async, &RendererMaster::Update, this);
			
		}

			int RendererMaster::Update()
			{
				try
				{
					while(shouldUpdateRendering)
					{						
						if(inputQueue.IsEmpty())
						{
							Idle();
							continue;
						}
						ExecuteNextFrame();										
					}				
				}
				catch(std::exception &e)
				{
					return 1;
				}
			
				return 0;
			
			}

				void RendererMaster::Idle()
				{
					{
						std::lock_guard<std::mutex> lock{ idleMutex };
						becameIdle = true;
					}	
								
					idleConditionVariable.notify_all();

					{
						std::lock_guard<std::mutex> lock{ idleMutex };
						becameIdle = false;
					}	
			
				}

				void RendererMaster::ExecuteNextFrame()
				{
					auto frame{ inputQueue.Pop() };
																	
					activeFrameHandle = std::async(std::launch::async, &FrameWorker::ExecuteCommands, &frame);					
					if(activeFrameHandle.get())
					{
						throw;
					}

					frame.WaitForCompletion();
					frame.ExecuteCommandPostGpuWork();
					outputQueue->Push(std::move(frame));
								
				}


		
		RendererMaster::~RendererMaster() noexcept
		{
			shouldUpdateRendering = false;
			
			if(updaterHandle.valid())
			{
				updaterHandle.wait();
			}
			
		}


				
		void RendererMaster::PropagateExceptions()
		{
			if(UpdaterThreadHasFinished())
			{
				const auto result{ updaterHandle.get() };
				if(result)
				{
					throw;
				}				
			}
			
		}

			bool RendererMaster::UpdaterThreadHasFinished() const
			{
				return updaterHandle.wait_for(std::chrono::seconds{0}) == std::future_status::ready;
			
			}

		

		void RendererMaster::ScheduleFrameWorker(FrameWorker &&frame)
		{
			inputQueue.Push(std::move(frame));
			
		}


		
		size_t RendererMaster::GetScheduledWorkerCount() const
		{
			return inputQueue.Size();
			
		}

		void RendererMaster::WaitForIdle()
		{
			std::unique_lock<std::mutex> lock{ idleMutex };
			idleConditionVariable.wait(lock, [&freshlyIdle = becameIdle](){ return freshlyIdle; });			
			
		}

		
	}
	
	
}