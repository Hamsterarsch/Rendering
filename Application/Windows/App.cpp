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
#include "Ui/ImguiTypeArithmetics.hpp"
#include "Windows/SelectPathDialog.hpp"


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

	
	static bool ButtonCentered(const char *label, bool centerVertical = true, bool centerHorizontal = true)
	{
		const auto buttonRect{ ImGui::GetStyle().FramePadding*2 + ImGui::CalcTextSize(label) };
		auto offset{ ( ImGui::GetContentRegionAvail() - buttonRect) *.5f };

		offset.x = centerHorizontal ? offset.x : 0;
		offset.y = centerVertical ? offset.y : 0;
		
		ImGui::SetCursorPos(ImGui::GetCursorPos() + offset);
				
		return ImGui::Button(label);
		
	}

		int F(ImGuiInputTextCallbackData *data)
		{
			switch(data->EventFlag)
			{
				case ImGuiInputTextFlags_CallbackResize:					
					reinterpret_cast<std::string *>(data->UserData)->resize(data->BufTextLen);
				break;
				case ImGuiInputTextFlags_CallbackCharFilter:
					switch(data->EventChar)
					{
						case '.': return 1;
						case '/': return 1;
						case '\\': return 1;
						case ' ': return 1;
						case '\t': return 1;
					}
				break;
			}
			return 0;
		}
	
		void App::Update()
		{		
			static ImGuiWindowFlags StaticWindowStyle{ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize };
		
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::SetNextWindowPos({0,0});			
			ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
			ImGui::Begin("OuterWindow", nullptr, StaticWindowStyle | ImGuiWindowFlags_NoDecoration);
			ImGui::PopStyleVar();
		
			auto outerSize{ ImGui::GetWindowSize() };
			auto innerSize{ outerSize * ImVec2{.5f, .25f} };
			
			auto innerPos{ (outerSize - innerSize) / 2.f };
			innerPos.y -= outerSize.y *.15f;
		
			ImGui::SetNextWindowSize(innerSize);
			ImGui::SetNextWindowPos(innerPos);
				ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(100, 144, 133, 255));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(30, 30, 30, 255));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, {.5, .5});
				ImGui::Begin("Open a project or create a new one to begin", nullptr, StaticWindowStyle);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor(2);		

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Columns(2, nullptr, false);

				const char *cnlabe{ "Create a new Project" };
				if(ButtonCentered("Create New"))
				{
					ImGui::OpenPopup(cnlabe);		
				}

				if(ImGui::IsPopupOpen(cnlabe))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, {.5, .5});
					bool b{ true };
					if(ImGui::BeginPopupModal(cnlabe, &b, StaticWindowStyle))
					{						
						ImGui::Spacing();						
						ImGui::Text("Select a folder where the project should be created");
												
						static std::string path{};
						if(ImGui::Button("Select Folder"))
						{
							::App::Windows::SelectPathDialog d{};
							path = d.GetSelectedItem().string();
							
						}
						const auto endPos{ ImGui::GetCursorPosX() + ImGui::GetItemRectSize().x };

						ImGui::SameLine(endPos, 5);
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::InputText(" ", path.data(), path.size(), ImGuiInputTextFlags_ReadOnly);
													
						
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
						ImGui::Text("Project Name: ");
						
						ImGui::SameLine(endPos, 5);
						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						static std::string str{};
						ImGui::InputText("lablee", str.data(), str.capacity(), ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackCharFilter, &F, &str);

						
						ImGui::Spacing();
						ImGui::Spacing();

						if(ButtonCentered("Confirm", false))
						{
							int e = 1;//do stuff
						}
						
						ImGui::EndPopup();
					}
					ImGui::PopStyleVar();
				}
		
				ImGui::NextColumn();
				if(ButtonCentered("Open Existing"))
				{
				}
					
				ImGui::End();
		
			ImGui::End();
			


		
			ImGui::ShowDemoWindow();
			
		}


	
	void App::ResizeMainWindow(int width, int height)
	{		
		renderer->FitWindowSurfaceToWindow(mainWindowSurface);
		
	}

	
}
