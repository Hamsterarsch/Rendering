#include "Windows/Application.hpp"
#include "Types/Dimensions2D.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "Ui/Core/UiBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui_impl_win32.h"
#include "Core/CreateProject.hpp"

#include "AssetConstructOperationsHelper.hpp"
#include "AssetTypes/ImageAsset.hpp"
#include "AssetTypes/PipelineAsset.hpp"
#include "AssetTypes/UserPixelShaderAsset.hpp"

#include "AssetTypes/StaticMeshAsset.hpp"
#include "AssetFactories/StaticMeshFactory.hpp"
#include "Rendering/GraphVisitorHarvestMeshes.hpp"
#include "Scene/ContentLight.hpp"





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
			ui{ *this },

			cube{ programAssets->GetAsset("Meshes/MetricCube.msh") },
			graphRoot{ Math::Matrix{}, {} }
		{
			std::filesystem::path includePath{ programAssets->GetAbsoluteRootAssetPath() };
			includePath /= "Shaders/Includes";					
			renderer->AddShaderIncludeDirectory(includePath.string().c_str());
					
			rendererMediator.SetMainWindowSurface(mainWindowSurface);
			rendererMediator.SetMainDepthTextureView(mainDepthTextureView);
		
			Assets::UserPixelShaderAsset::SetPixelShaderTemplate(programAssets->GetAsset("Shaders/LightingShaderTemplate.ps.shdr"));
						
			auto psoa = programAssets->GetAsset("DefaultAssets/DefaultPipelineState.pso");
						
			auto scale{ Math::Matrix::MakeScale(150, 150, 1)};					
			graphRoot.AddChild( Math::Matrix::MakeTranslation(0, 0, 500)*scale, MakeUnique<Scene::ContentMesh>(cube, psoa));			
			graphRoot.AddChild( Math::Matrix::MakeTranslation(0,0, 490), MakeUnique<Scene::ContentLight>(100, Math::Vector3{ 1, 1, 1 }, 0, 0) );
		
			currentHarvest = MakeUnique<Rendering::GraphVisitorHarvestMeshes>();
			graphRoot.Accept(*currentHarvest);
		
		}

			UniquePtr<Renderer::RendererFacade> Application::MakeRendererAndAddProgramShaderInclude(HWND window, assetSystem::AssetSystem &programAssets)
			{
				auto renderer{ Renderer::MakeRenderer(window) };

				std::filesystem::path includePath{ programAssets.GetAbsoluteRootAssetPath() };
				includePath /= "Shaders/Includes";					
				renderer->AddShaderIncludeDirectory(includePath.string().c_str());

				return renderer;
		
			}
			

	
	Application::Application(Application &&other) noexcept
	{
		*this = std::move(other);
		
	}

	
	
	Application &Application::operator=(Application &&rhs) noexcept
	{
		if(this == &rhs)
		{
			return *this;
			
		}
						
		programVersion		  = std::move(rhs.programVersion		);
		programAssets		  = std::move(rhs.programAssets			);
		projectAssets		  = std::move(rhs.projectAssets			);
		window				  = std::move(rhs.window				);
		renderer			  = std::move(rhs.renderer				);
		mainWindowSurface	  = std::move(rhs.mainWindowSurface		);
		mainDepthTexture	  = std::move(rhs.mainDepthTexture		);
		mainDepthTextureView  = std::move(rhs.mainDepthTextureView	);
		assetTypesRegistry	  = std::move(rhs.assetTypesRegistry	);
		rendererMediator	  =	std::move(rhs.rendererMediator	 	);
		ui					  = std::move(rhs.ui					);
		cube				  = std::move(rhs.cube					);
		graphRoot			  = std::move(rhs.graphRoot				);
		currentHarvest		  = std::move(rhs.currentHarvest		);

		rhs.programAssets.reset();

		return *this;
		
	}


	
	Application::~Application()
	{
		if(programAssets)
		{
			Assets::UserPixelShaderAsset::SetPixelShaderTemplate({});			
		}
		
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
				rendererMediator.SubmitFrame(currentHarvest);
				ImGui::EndFrame();//todo remove when ui render is submitting again
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


	
	void Application::ResizeMainWindow(const int width, const int height)
	{		
		mainDepthTexture = { renderer.get(), renderer->MakeDepthTexture(width, height, true) };
		mainDepthTextureView = { renderer.get(), renderer->GetViewFactory().MakeDepthTextureView(mainDepthTexture) };
		
		renderer->FitWindowSurfaceToWindow(mainWindowSurface);
		
		rendererMediator.OnMainWindowSurfaceSizeChanged({width, height});
		rendererMediator.SetMainDepthTextureView(mainDepthTextureView);
		
	}
	   
	
}
