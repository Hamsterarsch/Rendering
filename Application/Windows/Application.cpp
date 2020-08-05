#include "Windows/Application.hpp"
#include "Types/Dimensions2D.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "ThirdParty/imgui/imgui_impl_win32.h"
#include "Core/CreateProject.hpp"
#include "Rendering/GraphVisitorHarvestMeshes.hpp"



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
				Application::Get().ResizeMainWindow(width, height);
	        	
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
			mainDepthTexture{ renderer.get(), renderer->MakeDepthTexture(1280, 720, true) },
			mainDepthTextureView{ renderer.get(), renderer->GetViewFactory().MakeDepthTextureView(mainDepthTexture) },
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
			rendererMediator.SetMainDepthTextureView(mainDepthTextureView);
		
			Assets::UserPixelShaderAsset::SetPixelShaderTemplate(programAssets->GetAsset("Shaders/LightingShaderTemplate.ps.shdr"));
						
			auto psoa = programAssets->GetAsset("DefaultAssets/DefaultPipelineState.pso");

						
					
		}

			UniquePtr<Renderer::RendererFacade> Application::MakeRendererAndAddProgramShaderInclude(HWND window, assetSystem::AssetSystem &programAssets)
			{
				auto renderer{ Renderer::MakeRenderer(window) };

				std::filesystem::path includePath{ programAssets.GetAbsoluteRootAssetPath() };
				includePath /= "Shaders/Includes";					
				renderer->AddShaderIncludeDirectory(includePath.string().c_str());

				return renderer;
		
			}
			

	
	Application::~Application()
	{
		Assets::UserPixelShaderAsset::SetPixelShaderTemplate({});			
		
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
				
			}
		}
		ImGui_ImplWin32_Shutdown();
		
	}	

	
				bool IsKeyDown(int keycode)
				{
					return GetAsyncKeyState(keycode);
				}
	
		void Application::Update()
		{
			QueryUiInputAndSubmitUiRenderData();

			static Math::Vector3 rot{};
			if(ImGui::GetIO().MouseDown[1] && projectAssets)
			{				
				auto mouseDelta{ ImGui::GetIO().MouseDelta };
				constexpr float rotSpeed{ 0.5 };
				
				rot.x += mouseDelta.y*rotSpeed;
				rot.y += mouseDelta.x*rotSpeed;														
			}
			
			constexpr int Key_W{ 0x57 };
			constexpr int Key_A{ 0x41 };
			constexpr int Key_S{ 0x53 };
			constexpr int Key_D{ 0x44 };
			static Math::Vector3 pos{};
			if(not ImGui::GetIO().WantCaptureKeyboard && projectAssets)
			{				
				const float delta{ ImGui::GetIO().DeltaTime };
				const auto forwardVector{ Math::Matrix::MakeRotation(rot.x, rot.y, rot.z).Transform({0,0,1,1}) };										
				const Math::Vector3 rightVector{ forwardVector.z, 0, -forwardVector.x };					
								   					
				
				constexpr float speed{ 1 };
				if(IsKeyDown(Key_W))
				{
					pos += forwardVector*speed*delta;						
				}

				if(IsKeyDown(Key_S))
				{
					pos -= forwardVector*speed*delta;
				}

				if(IsKeyDown(Key_D))
				{
					pos += rightVector*speed*delta;
				}
				
				if(IsKeyDown(Key_A))
				{
					pos -= rightVector*speed*delta;
				}

		
				
			}
			rendererMediator.SetCurrentSceneView(pos, rot);		
		
			auto harvester{ MakeUnique<Rendering::GraphVisitorHarvestMeshes>() };
			scene.Accept(*harvester);
			 
			rendererMediator.SubmitFrame(harvester);
		
		}
	
			void Application::QueryUiInputAndSubmitUiRenderData()
			{
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
										
				ui.Update(builder);
		
			}



	void Application::ResizeMainWindow(const int width, const int height)
	{		
		mainDepthTexture = { renderer.get(), renderer->MakeDepthTexture(width, height, true) };
		mainDepthTextureView = { renderer.get(), renderer->GetViewFactory().MakeDepthTextureView(mainDepthTexture) };

		renderer->SubmitDefaultContextCommand();//make sure that the context command does not reference surface textures
		renderer->FitWindowSurfaceToWindow(mainWindowSurface);
		
		rendererMediator.OnMainWindowSurfaceSizeChanged({width, height});
		rendererMediator.SetMainDepthTextureView(mainDepthTextureView);
		
	}
	   
	
}
