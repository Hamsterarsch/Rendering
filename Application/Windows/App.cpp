#include "Types/Dimensions2D.hpp"
#include "DX12/Facade.hpp"
#include "Windows/App.hpp"

#include "Shared/Filesystem/Conversions.hpp"
#include <fstream>


#include "ThirdParty/glm/mat4x4.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Resources/SerializationContainer.hpp"

#include "Rendering/RendererMediator.hpp"
#include "StateSettings/SamplerSpec.hpp"
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_impl_win32.h"


// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Windows
{
	
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
	    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	        return true;
	
	    switch (msg)
	    {
	    case WM_SIZE:
	        if (wParam != SIZE_MINIMIZED)
	        {
				const auto width{ LOWORD(lParam) };
	        	const auto height{ HIWORD(lParam) };
				App::Get().ResizeMainWindow(height, width);
	        	
			}	           
	        return 0;
	    case WM_SYSCOMMAND:
	        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
	            return 0;
	        break;
	    case WM_DESTROY:
	        ::PostQuitMessage(0);
	        return 0;
	    }
	    return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}


	
	App &App::Get()
	{
		static App app{};
		return app;
		
	}

		App::App() :
			window{ {1280, 720}, L"Window", L"UniqueClassName", WndProc },
			renderer{ Renderer::MakeRenderer(window.GetHandle()) },
			mainWindowSurface{ renderer.get(), renderer->MakeWindowsWindowSurface(window.GetHandle()) },
			rendererMediator
			{			
				*renderer,
				{ rendererMediator, {1,1} },
				{ rendererMediator, *renderer }
			}
		{
			rendererMediator.SetMainWindowSurface(mainWindowSurface);			
		}

	
	
	void App::EnterLoop()
	{
		window.ShowWindow();
		ImGui_ImplWin32_Init(window.GetHandle());
		
		constexpr UINT NO_FILTER{ 0 };
		constexpr decltype(nullptr) FOR_ALL_WINDOWS{ nullptr };

		MSG msg{};
		while (msg.message != WM_QUIT)
		{
			const auto hasNewMessage
			{
				PeekMessage
				(
					&msg,
					FOR_ALL_WINDOWS,
					NO_FILTER,
					NO_FILTER,
					PM_REMOVE
				)
			};

			if (hasNewMessage)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{				
				Update();				
				rendererMediator.SubmitFrame();
			}
		}
		ImGui_ImplWin32_Shutdown();
		
	}	
	   	
		void App::Update()
		{
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::SetNextWindowPos({0,0});			;
			ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
			ImGui::Begin("Open Project", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);			
			ImGui::Text("Open Project");
			ImGui::End();
			
			
		}


	
	void App::ResizeMainWindow(int width, int height)
	{		
		renderer->FitWindowSurfaceToWindow(mainWindowSurface);
		
	}

	
}
