#include "Renderer.hpp"
#include "DX12/Facade.hpp"


namespace Renderer
{
	Renderer::Renderer(HWND outputWindow) :
		inflightFramesAmount{ 1 },
		shouldUpdateRendering{ false }
	{
		resources = RHA::DX12::Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_12_0, true);
		commonQueue = RHA::DX12::Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
		outputSurface = RHA::DX12::Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow);
		commonAllocator = RHA::DX12::Facade::MakeCmdAllocator(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
				
		updaterHandle = std::async( std::launch::async, &Renderer::UpdateRendering, this);

		{
			std::lock_guard<std::mutex> lock{ updaterMutex };
			shouldUpdateRendering = true;			
		}
		updaterCondition.notify_all();
			
	}

		int Renderer::UpdateRendering()
		{
			std::unique_lock<std::mutex> lock{ updaterMutex };
			updaterCondition.wait(lock, [&shouldUpdateRendering = shouldUpdateRendering] { return shouldUpdateRendering; });

			while(shouldUpdateRendering)
			{
				outputSurface->ScheduleBackbufferClear(commonQueue.get());

				//other commands
				
				outputSurface->SchedulePresentation(commonQueue.get());							
			}
			
			return 0;
		
		}

	Renderer::~Renderer()
	{
		{
			std::lock_guard<std::mutex> lock{ updaterMutex };
			shouldUpdateRendering = false;

			updaterHandle.wait();
		}
		
	}


	void Renderer::SubmitFrameInfo()
	{
		
				
	}

	
}

