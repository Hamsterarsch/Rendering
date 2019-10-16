#pragma once
#include "Window/Windows/Window.hpp"
#include "Renderer.hpp"


namespace Windows
{
	class App
	{
		private: Windows::Window window;

		private: Renderer::Renderer renderer;
		

		
		public: App();

		private: void Initialize();

		private: void Update();

	};

	
}
