#pragma once
#include <Windows.h>
#include <string>


class Dimensions2D;


namespace App::Windows
{	
	class Window
	{
		private: std::wstring windowName;
		
		private: std::wstring className;
		
		private: HWND handle;

				

		public: Window() = default;
		
		public: Window(const std::wstring &windowName, const std::wstring &className, WNDPROC windowProc = nullptr);
		
		public: Window(const Dimensions2D &size, const std::wstring &windowName, const std::wstring &className, WNDPROC windowProc = nullptr);
		
			private: void CreateWindowClass(WNDPROC windowProc) const;
		
			private: static LRESULT _stdcall Procedure(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
				
		public: Window(Window &&other) noexcept;
		
		public: Window &operator=(Window &&rhs) noexcept;
		
		public: ~Window();
		
		public: Window &operator=(const Window &) = delete;
		
		public: Window(const Window &) = delete;

		
		public: inline HWND GetHandle() const { return handle; }

		public: void SetWindowSize(const Dimensions2D &size);

		public: void ShowWindow();
				
	};

	
}
