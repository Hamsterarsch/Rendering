#include "Window.hpp"
#include "Types/Dimensions2D.hpp"
#include "Shared/Exception/CreationFailedException.hpp"

namespace Windows
{
	Window::Window(const std::wstring &windowName, const std::wstring &className, WNDPROC windowProc) :
		Window
		{
			{
				static_cast<unsigned>(GetSystemMetrics(SM_CXSCREEN)),
				static_cast<unsigned>(GetSystemMetrics(SM_CYSCREEN))
			},
			windowName,
			className,
			windowProc
		}
	{
	}

		Window::Window(const Dimensions2D &size, const std::wstring &windowName, const std::wstring &className, WNDPROC windowProc) :
			windowName{ windowName },
			className{ className }
		{
			CreateWindowClass(windowProc != nullptr ? windowProc : &Window::Procedure);
					
			constexpr decltype(nullptr) NO_PARENT{}, NO_MENU{};
			constexpr LPARAM NO_INIT_MSG{ 0 };

			const auto primaryDisplayWidth{ GetSystemMetrics(SM_CXSCREEN) };
			const auto primaryDisplayHeight{ GetSystemMetrics(SM_CYSCREEN) };

			const auto displacementX{ (primaryDisplayWidth - size.GetWidth()) / 2 };
			const auto displacementY{ (primaryDisplayHeight - size.GetHeight()) / 2 };
			
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
				WS_OVERLAPPED | WS_THICKFRAME,
				displacementX,
				displacementY,
				size.GetWidth(),
				size.GetHeight(),
				NO_PARENT, 
				NO_MENU, 
				GetModuleHandle(nullptr),
				NO_INIT_MSG
			);

			//SetWindowLong(handle, GWL_STYLE, 0);
		
			if(handle == nullptr)
			{
				throw Exception::CreationFailed("Could not create window");
			}

		}

			void Window::CreateWindowClass(WNDPROC windowProc) const
			{
			
				WNDCLASSW wclass{};
				wclass.cbClsExtra = sizeof(decltype(wclass));
				wclass.hInstance = GetModuleHandle(nullptr);
				wclass.lpszClassName = this->className.data();
				wclass.lpfnWndProc = windowProc;
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


	
	void Window::SetWindowSize(const Dimensions2D &size)
	{
		const auto primaryDisplayWidth{ GetSystemMetrics(SM_CXSCREEN) };
		const auto primaryDisplayHeight{ GetSystemMetrics(SM_CYSCREEN) };

		const auto displacementX{ (primaryDisplayWidth - size.GetWidth()) / 2 };
		const auto displacementY{ (primaryDisplayHeight - size.GetHeight()) / 2 };
		
		SetWindowPos(handle, HWND_TOP, displacementX, displacementY, size.GetWidth(), size.GetHeight(), 0);
		
	}


	
	void Window::ShowWindow()
	{
		::ShowWindow(handle, SW_SHOW);
		
	}
	
}
