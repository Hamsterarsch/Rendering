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
		ImGui_ImplDX12_NewFrame();//this only creates the same root signature over and over again
		ImGui_ImplWin32_NewFrame();//reads input, current window size, etc.
		ImGui::NewFrame();//various by frame updates, does not seem to interact with the rendering.	Imgui functions drawing ui count for the next frame after this call

		//App stuff for swap chain change and transitions / clearing goes here

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmd list)//draw data is what contains imgui state information related to rendering, because this function does the actual interaction with the gpu. maybe just work from here instead of replacing the rotten dx12 backend

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
