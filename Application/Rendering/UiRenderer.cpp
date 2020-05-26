#include "Rendering/UiRenderer.hpp"
#include "Rendering/RendererMediator.hpp"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_impl_win32.h"

//replace change this to be based on renderer cmds
#include "ThirdParty/imgui/imgui_impl_dx12.h"


namespace App::Rendering
{
	UiRenderer::UiRenderer(RendererMediator &mediator, void *windowHandle)
	{
		IMGUI_CHECKVERSION();
		
		imguiContext = ImGui::CreateContext();
		ImGui::StyleColorsDark();

		if(ImGui_ImplWin32_Init(windowHandle))
		{
			
		}

		ImGui_ImplDX12_Init()
		
	}

	void UiRenderer::SubmitFrame()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//App stuff for swap chain change and transitions / clearing goes here

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmd list)

		//close and submit list
		
	}

	UiRenderer::~UiRenderer()
	{
		//ensure resources can be freed

		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		
	}

	
}
