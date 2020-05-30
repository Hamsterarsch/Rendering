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
				 
		private: ::App::Rendering::RendererMediator rendererMediator;
		

		
		public: App();
				
			private: void Initialize();

			private: void Update();

	};

	
}
