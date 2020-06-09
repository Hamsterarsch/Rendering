#pragma once
#include "Window/Windows/Window.hpp"
#include "RendererFacade.hpp"
#include "Rendering/RendererMediator.hpp"
#include <forward_list>


namespace App::Ui{ class WidgetBase; }

namespace Windows
{
	class App
	{
		private: Windows::Window window;

		private: UniquePtr<Renderer::RendererFacade> renderer;

		private: Renderer::HandleWrapper mainWindowSurface;
		
		private: ::App::Rendering::RendererMediator rendererMediator;

		private: std::forward_list<UniquePtr<::App::Ui::WidgetBase>> widgets;

		
		
		public: static App &Get();
		
			private: App();

		
		public: void EnterLoop();

			private: void Update();

		public: void ResizeMainWindow(int width, int height);
		
	};

	
}
