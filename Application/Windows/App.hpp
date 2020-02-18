#pragma once
#include "Window/Windows/Window.hpp"
#include "Renderer.hpp"


namespace Windows
{
	class App
	{
		private: Windows::Window window;

		private: Renderer::DX12::Renderer renderer;

		private: size_t meshHandle, psoHandle, rootHandle, meshSize, meshBytesToIndices;

		private: size_t minstancePsoHandle;
		
		private: size_t transformBufferHandle;

		
		public: App();

		private: void Initialize();

		private: void Update();

	};

	
}
