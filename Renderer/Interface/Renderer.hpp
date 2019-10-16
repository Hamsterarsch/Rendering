#pragma once
#include "RendererExportHelper.hpp"



namespace Renderer
{
	class RENDERER_DLLSPEC Renderer
	{
		private: const unsigned inflightFramesAmount;


		
		public: Renderer(unsigned inflightFramesAmount);
		
		public: void SubmitNextFrameInfo(void *info);

		public: void WaitForCapacity();
		
		
	};
	
}
