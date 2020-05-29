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
	    {/*
	    case WM_SIZE:
	        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
	        {
	            WaitForLastSubmittedFrame();
	            ImGui_ImplDX12_InvalidateDeviceObjects();
	            CleanupRenderTarget();
	            ResizeSwapChain(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
	            CreateRenderTarget();
	            ImGui_ImplDX12_CreateDeviceObjects();
	        }
	        return 0;*/
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
	
	App::App() :
		window{ {1920, 1080}, true, L"Window", L"UniqueClassName", WndProc },
		renderer{ Renderer::MakeRenderer(window.GetHandle()) },
		rendererMediator
	{
		{ renderer.get(), renderer->MakeWindowsWindowSurface(window.GetHandle()) },
		*renderer,
		{ rendererMediator, {1,1} },
		{ rendererMediator, renderer.get() }
	}
	{
		//Initialize();
		auto *d = WndProc;
		
		//dearimgui render setup
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

	struct vertex
	{
		float x, y, z;
	};

		
		void App::Initialize()
		{
			/*
			struct
			{
				vertex vertexData[8]{ {-0.75, -0.75, 0 }, {0,0,-1}, {0.75, -0.75, 0}, {0,0,-1}, {0.75, 0.75, 0}, {0,0,-1}, { -0.75, 0.75, 0}, {0,0,-1} };
				unsigned indices[6]{ 0,1,2, 2,3,0 };
			} meshdata;
			meshSize = sizeof meshdata;
			meshBytesToIndices = sizeof meshdata.vertexData;

			meshHandle = renderer->MakeBuffer(&meshdata, sizeof(meshdata));
						

			Renderer::SerializeContainer root{};
			renderer->SerializeRootSignature(1, 3, 0, 0, &root);
			rootHandle = renderer->MakeRootSignature(root.GetData());

			Renderer::SerializeContainer ps{};
			{
				std::ifstream shaderFile{Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Lighting.ps"), std::ios_base::in | std::ios_base::ate };
				
				const auto charCount{ shaderFile.tellg() };
				shaderFile.seekg(0);

				auto pshader{ std::make_unique<char[]>(charCount) };
				shaderFile.read( pshader.get(), charCount);
							
				renderer->CompilePixelShader(pshader.get(), charCount, &ps);
								
			}


			Renderer::SerializeContainer vs{};
			{
				std::ifstream shaderFile{Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/LightingInstanced.vs"), std::ios_base::in | std::ios_base::ate };
					
				const auto charCount{ shaderFile.tellg() };
				shaderFile.seekg(0);

				auto pshader{ std::make_unique<char[]>(charCount) };
				shaderFile.read( pshader.get(), charCount);
							
				renderer->CompileVertexShader(pshader.get(), charCount, &vs);
								
			}
			

			{
				Renderer::ShaderList shaderList{};
				shaderList.vs.data = vs.GetData();
				shaderList.vs.sizeInBytes = vs.GetSize();

				shaderList.ps.data = ps.GetData();
				shaderList.ps.sizeInBytes = ps.GetSize();

				psoOpaqueShadedWithInstanceSupport = renderer->MakePso(Renderer::PipelineTypes::Opaque, Renderer::VertexLayoutTypes::PositionNormal, shaderList, rootHandle);
			}
			*/

			renderer->SetCamera(0, 0, -11, 0, 0, 0);
			
			renderer->MakeLight({0, 0, -1}, {0, 0, 0}, {1, 4, 4}, 3);
			renderer->MakeLight({4, 0, -2}, {0, 0, 0}, {6, 0, 0},  5);			
		
		}

	
	   	
		void App::Update()
		{
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
												
			ImGui::ShowDemoWindow();
			
			
		}

	
}
