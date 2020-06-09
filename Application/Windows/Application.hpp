#pragma once
#include "Window/Windows/Window.hpp"
#include "RendererFacade.hpp"
#include "Rendering/RendererMediator.hpp"
#include <forward_list>


namespace App::Ui{ class WidgetBase; }

namespace App::Windows
{
	class Application
	{
		private: Window window;

		private: UniquePtr<Renderer::RendererFacade> renderer;

		private: Renderer::HandleWrapper mainWindowSurface;
		
		private: Rendering::RendererMediator rendererMediator;

		private: std::forward_list<UniquePtr<Ui::WidgetBase>> widgets;

		
		
		public: static Application &Get();
		
			private: Application();

		
		public: void EnterLoop();

			private: void Update();

		public: void ResizeMainWindow(int width, int height);
		
	};

	
}
