#include "Renderer/RendererMaster.hpp"
#include "Shared/Exception/Exception.hpp"


namespace Renderer::DX12
{

	RendererMaster::RendererMaster(QueueConcurrent<FrameWorker> &outputQueue) :
		outputQueue{ &outputQueue },
		shouldProcessWorkers{ true }
	{
		updaterHandle = std::async(std::launch::async, &RendererMaster::Update, this);
		
	}

		RendererMaster::UpdaterInfo RendererMaster::Update()
		{
			try
			{
				while(shouldProcessWorkers)
				{					
					inputQueue.WaitForItems();
					ProcessNextWorker();										
				}				
			}
			catch(std::exception &e)
			{
				return { 1, e };
			}
		
			return {};
		
		}

			void RendererMaster::ProcessNextWorker()
			{					
				auto frame{ inputQueue.ExtractFront() };
																
				activeWorkerHandle = std::async(std::launch::async, &FrameWorker::ExecuteCommands, &frame);						
				if(activeWorkerHandle.get())
				{
					throw Exception::Exception{ "Encountered an error during worker execution" };
				}

				frame.WaitForCompletion();
				frame.ExecuteCommandPostGpuWork();

				outputQueue->Push(std::move(frame));
				inputQueue.Pop();
		
			}


	
	RendererMaster::~RendererMaster() noexcept
	{
		shouldProcessWorkers = false;
		
		if(updaterHandle.valid())
		{
			updaterHandle.wait();
		}
		
	}


			
	void RendererMaster::PropagateExceptions()
	{
		if(UpdaterThreadHasFinished())
		{
			const auto updaterInfo{ updaterHandle.get() };
			if(updaterInfo.result)
			{
				throw updaterInfo.exception;
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
		PropagateExceptions();		
		inputQueue.WaitForEmpty();

		
	}

	
}
