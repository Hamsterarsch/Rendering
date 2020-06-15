#include "Windows/Application.hpp"
#include "Types/Dimensions2D.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "Ui/User/StartupProjectDialog.hpp"
#include "Ui/Core/UiBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui_impl_win32.h"
#include "Core/Globals.hpp"
#include "Core/CreateProject.hpp"
#include "AssetTypes/ShaderAsset.hpp"

#include "AssetSystem/Interface/AssetConstructOperationsHelper.hpp"


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
			}
		{
			rendererMediator.SetMainWindowSurface(mainWindowSurface);

			bool hasVersionMismatch;
			Core::globals.programAssetSystem = Core::LoadProject(Filesystem::Conversions::MakeExeRelative(L"../../ProgramContent/ProgramContent.proj.asset").c_str(), hasVersionMismatch);

			Core::globals.programAssetSystem->RegisterAssetClass(Assets::ShaderAsset::GetAssetClassExtension(), MakeUnique<assetSystem::AssetConstructOperationsHelper<Assets::ShaderAsset>>());

			auto s = Core::globals.programAssetSystem->GetAsset("Shaders/Lighting.shdr");
		
			uiFrontends.push_back(MakeUnique<Ui::User::StartupProjectDialogFrontend>());
						
			
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
					
			for(auto &&frontend : uiFrontends)
			{
				frontend->Update(builder);			
			}
					
		}


	
	void Application::ResizeMainWindow(int width, int height)
	{		
		renderer->FitWindowSurfaceToWindow(mainWindowSurface);
		
	}

	
}
