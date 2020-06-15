#pragma once
#include "Window/Windows/Window.hpp"
#include "RendererFacade.hpp"
#include "Rendering/RendererMediator.hpp"
#include "Ui/UiStateMachine.hpp"


namespace App::Ui::Core
{
	class UiFrontend;
}


namespace App::Windows
{
	class Application
	{
		private: Window window;

		private: UniquePtr<Renderer::RendererFacade> renderer;

		private: Renderer::HandleWrapper mainWindowSurface;
		
		private: Rendering::RendererMediator rendererMediator;

		private: Ui::UiStateMachine ui;

		
		
		public: static Application &Get();
		
			private: Application();

		
		public: void EnterLoop();

			private: void Update();

		public: void ResizeMainWindow(int width, int height);

		
	};

	
}
