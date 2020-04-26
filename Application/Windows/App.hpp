#pragma once
#include "Window/Windows/Window.hpp"
#include "Renderer.hpp"


namespace Windows
{
	class App
	{
		private: Windows::Window window;

		private: UniquePtr<Renderer::Renderer> renderer;

		private: size_t meshHandle, psoHandle, rootHandle, meshSize, meshBytesToIndices;

		private: size_t minstancePsoHandle;
		

		
		public: App();

		private: void Initialize();

		private: void Update();

	};

	
}
