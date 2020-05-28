#include "Rendering/UiRenderer.hpp"
#include "Rendering/RendererMediator.hpp"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_impl_win32.h"

//replace change this to be based on renderer cmds
#include "ThirdParty/imgui/imgui_impl_dx12.h"
#include <vector>
#include "Renderer.hpp"
#include "Resources/SerializationContainer.hpp"
#include "Renderer/StateSettings/SamplerSpec.hpp"


namespace App::Rendering
{
	UiRenderer::UiRenderer(RendererMediator &mediator)
	{
		IMGUI_CHECKVERSION();
		
		imguiContext = ImGui::CreateContext();
		ImGui::StyleColorsDark();

		auto *renderer{ &mediator.Renderer()  };
		
		auto &depthSettings{ renderer->GetDepthStencilSettings() };
		depthSettings.SetEnableDepth(false);

		
		auto &blendSettings{ renderer->GetBlendSettings() };
		blendSettings.SetEnableBlend(true);
		
		blendSettings.SetBlendSrcAlpha(&Renderer::BlendSettings::TargetSrc);
		blendSettings.SetBlendInverseSrcAlpha(&Renderer::BlendSettings::TargetDst);
		
		blendSettings.SetBlendInverseSrcAlpha(&Renderer::BlendSettings::TargetSrcAlpha);
		blendSettings.SetBlendZero(&Renderer::BlendSettings::TargetDstAlpha);

		blendSettings.SetBlendOpAdd(&Renderer::BlendSettings::TargetBlendOpColor);
		blendSettings.SetBlendOpAdd(&Renderer::BlendSettings::TargetBlendOpAlpha);

		auto &rasterSettings{ renderer->GetRasterizerSettings() };
		rasterSettings.SetFrontIsCounterClockwise(false);

		auto &vertexLayoutSettings{ renderer->GetVertexLayoutSettings() };
		vertexLayoutSettings.AddLayoutElementDesc(&Renderer::SemanticTargets::TargetPosition, 0, &Renderer::FormatTargets::R32G32_Float, (UINT)IM_OFFSETOF(ImDrawVert, pos));
		vertexLayoutSettings.AddLayoutElementDesc(&Renderer::SemanticTargets::TargetTexcoord, 0, &Renderer::FormatTargets::R32G32_Float, (UINT)IM_OFFSETOF(ImDrawVert, uv));
		vertexLayoutSettings.AddLayoutElementDesc(&Renderer::SemanticTargets::TargetColor, 0, &Renderer::FormatTargets::R8G8B8A8_Norm, (UINT)IM_OFFSETOF(ImDrawVert, col));//todo: check offsets

		Renderer::SamplerSpec samplerSpec{};
		samplerSpec.addressU = samplerSpec.addressV = samplerSpec.addressW = &Renderer::AddressingTargets::AddressingModeWrap;
		samplerSpec.filter = &Renderer::FilterTargets::FilterMinMagMipLinear;

		Renderer::SerializeContainer root{};
		renderer->SerializeRootSignature(0, 1, 0, 0, root, &samplerSpec, 1);
		uiSignature = { renderer, renderer->MakeRootSignature(root.GetData()) };


			//font tex
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		uiFontTexture ={ renderer, renderer->MakeTexture(pixels, width, height) };

		//static_assert(sizeof(ImTextureID) >= 4, "Can't pack descriptor handle into TexID, 32-bit not supported yet.");
		//io.Fonts->TexID = (ImTextureID)imguiFontsTex; //should be okay to not set io.Fonts because its only a user payload
		
		static const char* vertexShader =
        "cbuffer vertexBuffer : register(b0) \
        {\
          float4x4 ProjectionMatrix; \
        };\
        struct VS_INPUT\
        {\
          float2 pos : POSITION;\
          float4 col : COLOR0;\
          float2 uv  : TEXCOORD0;\
        };\
        \
        struct PS_INPUT\
        {\
          float4 pos : SV_POSITION;\
          float4 col : COLOR0;\
          float2 uv  : TEXCOORD0;\
        };\
        \
        PS_INPUT main(VS_INPUT input)\
        {\
          PS_INPUT output;\
          output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
          output.col = input.col;\
          output.uv  = input.uv;\
          return output;\
        }";

		Renderer::SerializeContainer vs{};
		renderer->CompileVertexShader(vertexShader, strlen(vertexShader), &vs);

		static const char* pixelShader =
            "struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            SamplerState sampler0 : register(s0);\
            Texture2D texture0 : register(t0);\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
              float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
              return out_col; \
            }";

		Renderer::SerializeContainer ps{};
		renderer->CompilePixelShader(pixelShader, strlen(pixelShader), &ps);

		Renderer::ShaderList list{};
		list.vs.data = vs.GetData();
		list.vs.sizeInBytes = vs.GetSize();
		list.ps.data = ps.GetData();
		list.ps.sizeInBytes = ps.GetSize();
		
		uiPso = { renderer, renderer->MakePso(list, uiSignature) };
				
	}

	void UiRenderer::SubmitFrame()
	{		
		ImGui::Render();
		auto *imguiDrawData{ ImGui::GetDrawData() };

		//buffer creation
		std::vector<char> bufferContents;		

		static_assert(sizeof(char) == 1);
		const auto vertexDataSizeInBytes{ imguiDrawData->TotalVtxCount * sizeof(ImDrawVert) };
		const auto bufferSizeInBytes{ vertexDataSizeInBytes + imguiDrawData->TotalIdxCount * sizeof(ImDrawIdx) };
		bufferContents.reserve(bufferSizeInBytes);
		
		//imgui stores vertices/indices per draw list, so we have to copy it to a continuous memory block before upload
		auto *vertexMemory{ bufferContents.data() };
		auto *indexMemory{ vertexMemory + vertexDataSizeInBytes };
		for(unsigned drawListIndex{ 0 }; drawListIndex < imguiDrawData->CmdListsCount; ++drawListIndex)
		{
			const auto* drawList{ imguiDrawData->CmdLists[drawListIndex] };
			std::memcpy(vertexMemory , drawList->VtxBuffer.Data, drawList->VtxBuffer.size_in_bytes());
			std::memcpy(indexMemory, drawList->IdxBuffer.Data, drawList->IdxBuffer.size_in_bytes());

			vertexMemory += drawList->VtxBuffer.size_in_bytes();
			indexMemory += drawList->IdxBuffer.size_in_bytes();				
		}
		uiVertexIndexBuffer = { &mediator->Renderer(), mediator->Renderer().MakeBuffer(bufferContents.data(), bufferContents.size()) };

		
		//cbv creation
		float L = imguiDrawData->DisplayPos.x;
		float R = imguiDrawData->DisplayPos.x + imguiDrawData->DisplaySize.x;
		float T = imguiDrawData->DisplayPos.y;
		float B = imguiDrawData->DisplayPos.y + imguiDrawData->DisplaySize.y;
		float orthogonalProjection[4][4]
		{
		    { 2.0f/(R-L),   0.0f,           0.0f,       0.0f },
		    { 0.0f,         2.0f/(T-B),     0.0f,       0.0f },
		    { 0.0f,         0.0f,           0.5f,       0.0f },
		    { (R+L)/(L-R),  (T+B)/(B-T),    0.5f,       1.0f },
		};		    

		auto cbvSizeInBytes{ sizeof(float) * 4 * 4 };
		uiConstantBuffer = { &mediator->Renderer(), mediator->Renderer().MakeBuffer(&orthogonalProjection, cbvSizeInBytes) };


		//descriptors
		auto &viewFactory{ mediator->Renderer().GetViewFactory() };
		viewFactory.DeclareNewDescriptorBlock(uiSignature, 2, 0);
		viewFactory.CreateConstantBufferView(uiConstantBuffer, 1, cbvSizeInBytes);
		viewFactory.CreateShaderResourceView(uiFontTexture, 1);
		
		uiDescriptors = { &mediator->Renderer(), viewFactory.FinalizeDescriptorBlock() };

		
		//submit commands for ui rendering
		const auto clipOffset{ imguiDrawData->DisplayPos };
		unsigned vertexOffset{ 0 };
		unsigned indexOffset{ 0 };

		for(unsigned drawListIndex{ 0 }; drawListIndex < imguiDrawData->CmdListsCount; ++drawListIndex)
		{
			const auto *drawList{ imguiDrawData->CmdLists[drawListIndex] };
			for(unsigned drawCommandIndex{ 0 }; drawCommandIndex < drawList->CmdBuffer.Size; ++drawCommandIndex)
			{
				const auto &drawCommand{ drawList->CmdBuffer[drawCommandIndex] };
				if(drawCommand.UserCallback != nullptr)
				{
					throw;
				}

				//set new scissor rect
				//set table
				//draw
				
			}
		}
			
	}
				
		//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmd list)//draw data is what contains imgui state information related to rendering, because this function does the actual interaction with the gpu. maybe just work from here instead of replacing the rotten dx12 backend

		//close and submit list
		
	

	UiRenderer::~UiRenderer()
	{
		//ensure resources can be freed

		ImGui_ImplDX12_Shutdown();

		ImGui::DestroyContext();
		
	}

	
}
