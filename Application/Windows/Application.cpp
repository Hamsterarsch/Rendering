#include "Windows/Application.hpp"
#include "Types/Dimensions2D.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "Ui/Core/UiBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui_impl_win32.h"
#include "Core/CreateProject.hpp"

#include "AssetConstructOperationsHelper.hpp"
#include "AssetTypes/ImageAsset.hpp"





// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace App::Windows
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
				Application::Get().ResizeMainWindow(height, width);
	        	
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

	
	Application &Application::Get()
	{
		static Application app{};
		return app;
		
	}

		Application::Application() :
			window{ {1280, 720}, L"Window", L"UniqueClassName", WndProc },
			renderer{ Renderer::MakeRenderer(window.GetHandle()) },
			mainWindowSurface{ renderer.get(), renderer->MakeWindowsWindowSurface(window.GetHandle()) },
			rendererMediator
			{			
				*renderer,
				{ rendererMediator, {1,1} },
				{ rendererMediator, *renderer }
			},
			programVersion{ 0, 0, 0 },
			programAssets{ LoadProject(Filesystem::Conversions::MakeExeRelative("../../ProgramContent/ProgramContent.proj.asset"), programVersion) },
			ui{ *this }
		{
			renderer->AddShaderIncludeDirectory(Filesystem::Conversions::MakeExeRelative("../../Content/Shaders/Includes").c_str());
			programAssets->RegisterAssetClass("img", MakeUnique<assetSystem::AssetConstructOperationsHelper<Assets::ImageAsset>>());
			rendererMediator.SetMainWindowSurface(mainWindowSurface);
			
		}



	void Application::SetProjectAssets(UniquePtr<assetSystem::AssetSystem> &&assets)
	{
		projectAssets = std::move(assets);
		
		assetTypesRegistry = Assets::AssetTypesRegistry{ *this,  programAssets->IsSameRootAssetPath(projectAssets->GetRootAssetPath().c_str()) };
		
	}


	
	void Application::EnterLoop()
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

	

	
		void Application::Update()
		{		
			static Ui::Core::UiBuilderImpl builder{};
		
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
					
			ui.Update(builder);
			
					
		}


	
	void Application::ResizeMainWindow(int width, int height)
	{		
		renderer->FitWindowSurfaceToWindow(mainWindowSurface);
		
	}


	
	
}
