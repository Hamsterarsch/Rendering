#pragma once
#include <Windows.h>
#include <string>


class Dimensions2D;

namespace Windows
{
	class Window
	{
		private: const std::wstring windowName;
		private: const std::wstring className;
		private: HWND handle;
		
		
		public: Window(const Dimensions2D &size, const std::wstring &windowName, const std::wstring &className);
			private: void CreateWindowClass() const;
			private: static LRESULT _stdcall Procedure(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
		
		public: ~Window();
		
		public: Window(Window &&Other);
		
		public: Window(const Window &) = delete;
		public: Window &operator=(const Window &) = delete;
		public: Window &operator=(Window &&) = delete;

		HWND GetHandle() const;
		
				
	};

	
}
