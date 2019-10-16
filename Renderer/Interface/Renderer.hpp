#pragma once
#include <thread>
#include <condition_variable>
#include <future>
#include "RendererExportHelper.hpp"



namespace Renderer
{
	class RENDERER_DLLSPEC Renderer
	{
		private: const unsigned inflightFramesAmount;

		private: std::mutex updaterMutex;

		private: std::condition_variable_any updaterCondition;

		private: bool shouldUpdateRendering;
		
		private: std::future<int> updaterHandle;




		
		public: Renderer(unsigned inflightFramesAmount);
		
			private: int UpdateRendering();
		
		public: void SubmitNextFrameInfo();

		public: void WaitForCapacity();

		
		
	};
	
}
