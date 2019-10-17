#pragma once
#include <Windows.h>
#include <condition_variable>
#include <future>
#include "RendererExportHelper.hpp"
#include "Shared/PtrTypes.hpp"


namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
		class Queue;
		class CmdAllocator;
		class CmdList;
		class WindowSurface;
		class Fence;
	}
}


namespace Renderer
{

	
	class RENDERER_DLLSPEC Renderer
	{
		private: size_t frameCounter;
		
		private: const unsigned inflightFramesAmount;

		private:std::vector<UniquePtr<RHA::DX12::Fence>> inflightFences;

		private: std::vector<UniquePtr<RHA::DX12::CmdList>> clearCommands;
		
		private: std::mutex updaterMutex;

		private: std::condition_variable updaterCondition;

		private: bool shouldUpdateRendering;
		
		private: std::future<int> updaterHandle;

		private: UniquePtr<RHA::DX12::DeviceResources> resources;

		private: UniquePtr<RHA::DX12::Queue> commonQueue;

		private: UniquePtr<RHA::DX12::CmdAllocator> commonAllocator;
		
		private: UniquePtr<RHA::DX12::WindowSurface> outputSurface;

				 		
		
		public: Renderer(HWND outputWindow, unsigned inflightFramesAmount);

			private: int UpdateRendering();

		public: ~Renderer();

					
		public: void SubmitFrameInfo();

		public: void WaitForCapacity();

		
		
	};
	
}
