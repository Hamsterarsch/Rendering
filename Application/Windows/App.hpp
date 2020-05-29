#pragma once
#include "Window/Windows/Window.hpp"
#include "Renderer.hpp"
#include "Rendering/RendererMediator.hpp"


namespace Windows
{
	class App
	{
		private: Windows::Window window;

		private: UniquePtr<Renderer::Renderer> renderer;
				 
		private: ::App::Rendering::RendererMediator rendererMediator;
		

		
		public: App();
				
			private: void Initialize();

			private: void Update();

	};

	
}
