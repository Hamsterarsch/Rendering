#pragma once
#include "Window/Windows/Window.hpp"


namespace Windows
{
	class App
	{
		private: Windows::Window window;


		
		public: App();

		private: void Initialize();

		private: void Update();

	};

	
}
