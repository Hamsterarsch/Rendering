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
		private: const unsigned inflightFramesAmount;
		
		private: std::mutex updaterMutex;

		private: std::condition_variable updaterCondition;

		private: bool shouldUpdateRendering;
		
		private: std::future<int> updaterHandle;

		private: UniquePtr<RHA::DX12::DeviceResources> resources;

		private: UniquePtr<RHA::DX12::Queue> commonQueue;

		private: UniquePtr<RHA::DX12::CmdAllocator> commonAllocator;
		
		private: UniquePtr<RHA::DX12::WindowSurface> outputSurface;

				 		
		
		public: Renderer(HWND outputWindow);

			private: int UpdateRendering();

		public: ~Renderer();

					
		public: void SubmitFrameInfo();

		public: void WaitForCapacity();

		
		
	};
	
}
