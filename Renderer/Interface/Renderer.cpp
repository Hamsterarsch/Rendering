#include "Renderer.hpp"
#include "DX12/Facade.hpp"


namespace Renderer
{
	Renderer::Renderer(HWND outputWindow, const unsigned inflightFramesAmount) :
		frameCounter{ 0 },
		inflightFramesAmount{ inflightFramesAmount },
		clearCommands(inflightFramesAmount),
		inflightFences(inflightFramesAmount),
		shouldUpdateRendering{ false }
	{
		resources = RHA::DX12::Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_12_0, true);
		commonQueue = RHA::DX12::Facade::MakeQueue(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
		outputSurface = RHA::DX12::Facade::MakeWindowSurface(resources.get(), commonQueue.get(), outputWindow);
		commonAllocator = RHA::DX12::Facade::MakeCmdAllocator(resources.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
		

		for(unsigned frameIndex{ 0 }; frameIndex < inflightFramesAmount; ++frameIndex)
		{			
			clearCommands.at(frameIndex) = commonAllocator->AllocateList();
			auto asGraphicsList{ clearCommands.at(frameIndex)->AsGraphicsList() };
			outputSurface->ClearBuffer(asGraphicsList, (1 + frameIndex) % outputSurface->GetBufferCount());			
			asGraphicsList->Close();
			
			inflightFences.at(frameIndex) = RHA::DX12::Facade::MakeFence(resources.get());
		}
		
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
				auto index{ frameCounter % inflightFramesAmount };
									
				while (inflightFences.at(index)->GetValue() == 1)
				{						
				}
				outputSurface->Present();

				
				inflightFences.at(index)->Signal(1);//signal frame begin

				commonQueue->SubmitCommandList(clearCommands.at(index).get());//submissions		

				commonQueue->GetQueue()->Signal(inflightFences.at(index)->GetFence().Get(), 0);//signal frame end
				

				++frameCounter;
				
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

