#include "Windows/Application.hpp"
#include "Types/Dimensions2D.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "Ui/Core/UiBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui_impl_win32.h"
#include "Core/CreateProject.hpp"

#include "AssetConstructOperationsHelper.hpp"
#include "AssetTypes/ImageAsset.hpp"
#include "AssetTypes/PsoAsset.hpp"
#include "AssetTypes/UserPixelShaderAsset.hpp"





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
			programVersion{ 0, 0, 0 },
			programAssets{ LoadProject(Filesystem::Conversions::MakeExeRelative("../../ProgramContent/ProgramContent.proj.asset"), programVersion) },
			window{ {1280, 720}, L"Window", L"UniqueClassName", WndProc },
			renderer{ MakeRendererAndAddProgramShaderInclude(window.GetHandle(), *programAssets) },
			mainWindowSurface{ renderer.get(), renderer->MakeWindowsWindowSurface(window.GetHandle()) },
			assetTypesRegistry{ *programAssets, *renderer },
			rendererMediator
			{			
				*renderer,
				{ rendererMediator, *renderer, *programAssets, {1280, 720} },
				{ rendererMediator, *renderer }
			},
			ui{ *this }
		{
			std::filesystem::path includePath{ programAssets->GetAbsoluteRootAssetPath() };
			includePath /= "Shaders/Includes";					
			renderer->AddShaderIncludeDirectory(includePath.string().c_str());
					
			rendererMediator.SetMainWindowSurface(mainWindowSurface);
			
			Assets::UserPixelShaderAsset::SetPixelShaderTemplate(programAssets->GetAsset("Shaders/LightingShaderTemplate.ps.shdr"));
		
		}

			UniquePtr<Renderer::RendererFacade> Application::MakeRendererAndAddProgramShaderInclude(HWND window, assetSystem::AssetSystem &programAssets)
			{
				auto renderer{ Renderer::MakeRenderer(window) };

				std::filesystem::path includePath{ programAssets.GetAbsoluteRootAssetPath() };
				includePath /= "Shaders/Includes";					
				renderer->AddShaderIncludeDirectory(includePath.string().c_str());

				return renderer;
		
			}



	void Application::SetProjectAssets(UniquePtr<assetSystem::AssetSystem> &&assets)
	{
		projectAssets = std::move(assets);
		assetTypesRegistry.RegisterAssetTypesWith(*projectAssets);
		assetTypesRegistry.SetShouldShowAllTypes(programAssets->IsSameRootAssetPath(projectAssets->GetAbsoluteRootAssetPath().c_str()));
				
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
