#include "Renderer/RendererMaster.hpp"


namespace Renderer
{
	namespace DX12
	{
		RendererMaster::RendererMaster(QueueConcurrent<FrameRenderer> &outputQueue, unsigned char maxScheduledFrames) :
			outputQueue{ &outputQueue },
			shouldUpdateRendering{ true },
			maxScheduledFrames{ maxScheduledFrames }
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
				}

				void RendererMaster::ExecuteNextFrame()
				{
					auto frame{ inputQueue.Pop() };
																	
					activeFrameHandle = std::async(std::launch::async, &FrameRenderer::ExecuteCommands, &frame);					
					if(activeFrameHandle.get())
					{
						throw;
					}

					frame.WaitForCompletion();
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

		

		void RendererMaster::ScheduleFrame(FrameRenderer &&frame)
		{
			if(this->HasNoCapacityForFrames())
			{
				return;
			}
			
			inputQueue.Push(std::move(frame));
			
		}
		
			bool RendererMaster::HasNoCapacityForFrames() const
			{
				return inputQueue.Size() >= maxScheduledFrames;
				
			}
			
		
	}
	
	
}