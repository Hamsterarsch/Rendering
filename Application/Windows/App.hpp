#pragma once
#include "Window/Windows/Window.hpp"
#include "RendererFacade.hpp"
#include "Rendering/RendererMediator.hpp"


namespace Windows
{
	class App
	{
		private: Windows::Window window;

		private: UniquePtr<Renderer::RendererFacade> renderer;

		private: Renderer::HandleWrapper mainWindowSurface;
		
		private: ::App::Rendering::RendererMediator rendererMediator;
		

		public: static App &Get();
		
			private: App();

		
		public: void EnterLoop();

			private: void Update();

		public: void ResizeMainWindow(int width, int height);
		
	};

	
}
