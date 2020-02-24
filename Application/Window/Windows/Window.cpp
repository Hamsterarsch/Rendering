#include "Window.hpp"
#include "Types/Dimensions2D.hpp"
#include "Shared/Exception/CreationFailedException.hpp"

namespace Windows
{
	Window::Window(const Dimensions2D &size, const bool isBorderlessFullscreen, const std::wstring &windowName, const std::wstring &className) :
		windowName{ windowName },
		className{ className },
		isFullscreen{ isBorderlessFullscreen }
	{
		CreateWindowClass();

		constexpr unsigned NO_X_DISP{ 0 }, NO_Y_DISP{ 0 };
		constexpr decltype(nullptr) NO_PARENT{}, NO_MENU{};
		constexpr LPARAM NO_INIT_MSG{ 0 };

		const auto primaryDisplayWidth{ GetSystemMetrics(SM_CXSCREEN) };
		const auto primaryDisplayHeight{ GetSystemMetrics(SM_CYSCREEN) };

		/*
		if(isBorderlessFullscreen)
		{
			DEVMODE primaryDisplaySettings{};
			primaryDisplaySettings.dmSize = sizeof primaryDisplaySettings;
			primaryDisplaySettings.dmPelsWidth = primaryDisplayWidth;
			primaryDisplaySettings.dmPelsHeight = primaryDisplayHeight;
			primaryDisplaySettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&primaryDisplaySettings, CDS_FULLSCREEN);
		}*/
		
		handle =
		CreateWindow
		(
			className.data(),
			this->windowName.data(),
			isBorderlessFullscreen ? 0 : WS_OVERLAPPEDWINDOW,
			NO_X_DISP,
			NO_Y_DISP,
			isBorderlessFullscreen ? primaryDisplayWidth : size.GetWidth(),
			isBorderlessFullscreen ? primaryDisplayHeight : size.GetHeight(),
			NO_PARENT, 
			NO_MENU, 
			GetModuleHandle(nullptr),
			NO_INIT_MSG
		);

		if(handle == nullptr)
		{
			throw Exception::CreationFailed("Could not create window");
		}
		ShowWindow(handle, SW_SHOW);
		
	}

		void Window::CreateWindowClass() const
		{
			WNDCLASSW wclass{};
			wclass.cbClsExtra = sizeof(decltype(wclass));
			wclass.hInstance = GetModuleHandle(nullptr);
			wclass.lpszClassName = this->className.data();
			wclass.lpfnWndProc = &Window::Procedure;
			wclass.style = CS_OWNDC;

			RegisterClass(&wclass);
		
		}

		LRESULT _stdcall Window::Procedure(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			switch(msg)
			{
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(handle, msg, wparam, lparam);
			}

			return 0;
		
		}

	Window::~Window()
	{
		if(handle != nullptr)
		{
			UnregisterClass(className.data(), GetModuleHandle(nullptr));			
		}
				
	}

	Window::Window(Window &&Other) :
		windowName{ std::move(Other.windowName) },
		className{ std::move(Other.className) },
		handle{ Other.handle }
	{
		Other.handle = nullptr;
		
	}

		
}
