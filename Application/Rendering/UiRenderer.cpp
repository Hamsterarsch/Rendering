#include "Rendering/UiRenderer.hpp"
#include "Rendering/RendererMediator.hpp"

#include "ThirdParty/imgui/imgui.h"


#include <vector>
#include "RendererFacade.hpp"
#include "Commands/CompositeCommand.hpp"
#include "Resources/SerializeTarget.hpp"
#include "StateSettings/SamplerSpec.hpp"


namespace App::Rendering
{
	UiRenderer::UiRenderer(RendererMediator &mediator, ::Renderer::RendererFacade &renderer) :
		mediator{ &mediator },
		imguiContext{ nullptr },
		submitDrawData{ nullptr },
		vertexIndexBufferSizeInBytes{ 0 },
		constantBufferSizeInBytes{ 0 }
	{
		IMGUI_CHECKVERSION();		
		imguiContext = ImGui::CreateContext();
		ImGui::StyleColorsDark();
		
		CreateUiSignature(renderer);			
		CreateUiFontTexture(renderer);
		CreateUiPipeline(renderer);
					
	}

		void UiRenderer::CreateUiSignature(::Renderer::RendererFacade &renderer)
		{
			SamplerSpec samplerSpec{};
			samplerSpec.addressU = samplerSpec.addressV = samplerSpec.addressW = &AddressingTargets::AddressingModeWrap;
			samplerSpec.filter = &FilterTargets::FilterMinMagMipLinear;

			SerializeTarget root{};
			renderer.SerializeRootSignature(1, 1, 0, 0, root, &samplerSpec, 1);
			uiSignature = { &renderer, renderer.MakeRootSignature(root.GetData(), root.GetSizeInBytes(), 0) };
		
		}

		void UiRenderer::CreateUiFontTexture(::Renderer::RendererFacade &renderer)
		{
			ImGuiIO &io{ ImGui::GetIO() };
			unsigned char *pixels;
			int width, height;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

			uiFontTexture = { &renderer, renderer.MakeTexture(pixels, width, height) };
		
		}

		void UiRenderer::CreateUiPipeline(::Renderer::RendererFacade &renderer)
		{
			renderer.GetDepthStencilSettings()
			.SetEnableDepth(false);
		
			
			renderer.GetBlendSettings()
			.SetEnableBlend(true)			
			.SetBlendSrcAlpha(&BlendSettings::TargetSrc)
			.SetBlendInverseSrcAlpha(&BlendSettings::TargetDst)
			
			.SetBlendInverseSrcAlpha(&BlendSettings::TargetSrcAlpha)
			.SetBlendZero(&BlendSettings::TargetDstAlpha)

			.SetBlendOpAdd(&BlendSettings::TargetBlendOpColor)
			.SetBlendOpAdd(&BlendSettings::TargetBlendOpAlpha);

		
			renderer.GetRasterizerSettings()
			.SetFrontIsCounterClockwise(false);

		
			renderer.GetVertexLayoutSettings()
			.AddLayoutElementDesc(&SemanticTargets::TargetPosition, 0, &FormatTargets::R32G32_Float, (UINT)IM_OFFSETOF(ImDrawVert, pos))
			.AddLayoutElementDesc(&SemanticTargets::TargetTexcoord, 0, &FormatTargets::R32G32_Float, (UINT)IM_OFFSETOF(ImDrawVert, uv))
			.AddLayoutElementDesc(&SemanticTargets::TargetColor, 0, &FormatTargets::R8G8B8A8_UNorm, (UINT)IM_OFFSETOF(ImDrawVert, col));


			auto vs{ CreateUiVertexShader(renderer) };
			auto ps{ CreateUiPixelShader(renderer) };
		
			ShaderList list{};
			list.vs.data = vs.GetData();
			list.vs.sizeInBytes = vs.GetSizeInBytes();
			list.ps.data = ps.GetData();
			list.ps.sizeInBytes = ps.GetSizeInBytes();
		
		
			uiPso = { &renderer, renderer.MakePso(list, uiSignature) };
		
		}

			SerializeTarget UiRenderer::CreateUiVertexShader(::Renderer::RendererFacade &renderer)
			{			
				static const char* vertexShader =
				"cbuffer vertexBuffer : register(b2)\
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

				SerializeTarget vs{};
				renderer.CompileVertexShader(vertexShader, strlen(vertexShader), vs);
				return vs;
			
			}

			SerializeTarget UiRenderer::CreateUiPixelShader(::Renderer::RendererFacade &renderer)
			{
				static const char *pixelShader =
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

				SerializeTarget ps{};
				renderer.CompilePixelShader(pixelShader, strlen(pixelShader), ps);

				return ps;
			
			}



	UiRenderer::~UiRenderer()
	{
		if(this->IsValid())
		{
			Free();
		}
		
	}
	
		bool UiRenderer::IsValid() const
		{
			return imguiContext != nullptr;
		
		}

			void UiRenderer::Free()
			{
				ImGui::DestroyContext();				
		
			}



	UiRenderer::UiRenderer(UiRenderer &&other) noexcept
	{
		*this = std::move(other);
		
	}
	   	
		UiRenderer &UiRenderer::operator=(UiRenderer &&other) noexcept
		{
			if(this == &other)
			{
				return *this;
				
			}

			if(this->IsValid())
			{
				this->Free();				
			}
		
			mediator = std::move(other.mediator);
			imguiContext = std::move(other.imguiContext);
			uiSignature = std::move(other.uiSignature);
			uiPso = std::move(other.uiPso);
			uiFontTexture = std::move(other.uiFontTexture);
			uiVertexIndexBuffer = std::move(other.uiVertexIndexBuffer);
			uiConstantBuffer = std::move(other.uiConstantBuffer);
			uiDescriptors = std::move(other.uiDescriptors);
			submitDrawData = std::move(other.submitDrawData);
			vertexIndexBufferSizeInBytes = std::move(other.vertexIndexBufferSizeInBytes);
			lastSubmitDisplayPos = std::move(other.lastSubmitDisplayPos);
			lastSubmitDisplaySize = std::move(other.lastSubmitDisplaySize);
			constantBufferSizeInBytes = std::move(other.constantBufferSizeInBytes);		
			drawRecordVertexOffset = std::move(other.drawRecordVertexOffset);
			drawRecordIndexOffset = std::move(other.drawRecordIndexOffset);
		
			other.Invalidate();

			return *this;
		
		}

			void UiRenderer::Invalidate()
			{
				imguiContext = nullptr;
			
			}

	

	void UiRenderer::SubmitFrame()
	{		
		ImGui::Render();		
		submitDrawData = ImGui::GetDrawData();
		
		if(ScreenIsMinimized())
		{
			return;
			
		}

		if(DrawDataIsEmpty())
		{
			auto uiReferenceDummy{ MakeUnique<Commands::CompositeCommand>() };
			uiReferenceDummy->AddReferenceTo(uiFontTexture);
			if(uiConstantBuffer.IsValid())
			{
				uiReferenceDummy->AddReferenceTo(uiConstantBuffer);				
			}
			if(uiDescriptors.IsValid())
			{
				uiReferenceDummy->AddReferenceTo(uiDescriptors);
			}
			
			mediator->Renderer().SubmitCommand(std::move(uiReferenceDummy));			
			return;//todo: do something against destroyed ui resources in this case other than fake commands
			
		}
		
		CreateBufferWithUiVertexIndexData();
		
		if(TryToUpdateUiConstantBuffer())
		{
			CreateDescriptorBlockForUiResources();
		}
			   
		SubmitUiRendererWork();	
		submitDrawData = nullptr;
		
	}
	
		bool UiRenderer::ScreenIsMinimized() const
		{
			return submitDrawData->DisplaySize.x <= 0 || submitDrawData->DisplaySize.y <= 0;
		
		}

		bool UiRenderer::DrawDataIsEmpty() const
		{
			return submitDrawData->TotalVtxCount == 0 || submitDrawData->TotalIdxCount == 0;
			
		}

		void UiRenderer::CreateBufferWithUiVertexIndexData()
		{			
			static_assert(sizeof(char) == 1);
			std::vector<char> bufferContents;		

			vertexIndexBufferSizeInBytes = submitDrawData->TotalVtxCount * sizeof(ImDrawVert);
			const auto bufferSizeInBytes{ vertexIndexBufferSizeInBytes + submitDrawData->TotalIdxCount * sizeof(ImDrawIdx) };
			bufferContents.resize(bufferSizeInBytes);

		
			//imgui stores vertices/indices per draw list, so we have to copy it to a continuous memory block before upload
			auto *vertexMemory{ bufferContents.data() };
			auto *indexMemory{ vertexMemory + vertexIndexBufferSizeInBytes };
			for(unsigned drawListIndex{ 0 }; drawListIndex < submitDrawData->CmdListsCount; ++drawListIndex)
			{
				const auto* drawList{ submitDrawData->CmdLists[drawListIndex] };
				std::memcpy(vertexMemory , drawList->VtxBuffer.Data, drawList->VtxBuffer.size_in_bytes());
				std::memcpy(indexMemory, drawList->IdxBuffer.Data, drawList->IdxBuffer.size_in_bytes());

				vertexMemory += drawList->VtxBuffer.size_in_bytes();
				indexMemory += drawList->IdxBuffer.size_in_bytes();				
			}
			uiVertexIndexBuffer = { &mediator->Renderer(), mediator->Renderer().MakeBuffer(bufferContents.data(), bufferContents.size()) };
		
		}

		bool UiRenderer::TryToUpdateUiConstantBuffer()
		{
			if(ConstantBufferValuesHaveNotChanged() && uiConstantBuffer.IsValid())
			{
				return false;
				
			}
					
			float L = submitDrawData->DisplayPos.x;
			float R = submitDrawData->DisplayPos.x + submitDrawData->DisplaySize.x;
			float T = submitDrawData->DisplayPos.y;
			float B = submitDrawData->DisplayPos.y + submitDrawData->DisplaySize.y;
			float orthogonalProjection[4][4]
			{
			    { 2.0f/(R-L),   0.0f,           0.0f,       0.0f },
			    { 0.0f,         2.0f/(T-B),     0.0f,       0.0f },
			    { 0.0f,         0.0f,           0.5f,       0.0f },
			    { (R+L)/(L-R),  (T+B)/(B-T),    0.5f,       1.0f },
			};		    

			constantBufferSizeInBytes = { sizeof(float) * 4 * 4 };
			uiConstantBuffer = { &mediator->Renderer(), mediator->Renderer().MakeBuffer(&orthogonalProjection, constantBufferSizeInBytes) };

			return true;
		
		}

			bool UiRenderer::ConstantBufferValuesHaveNotChanged() 
			{
				const auto haveValuesChanged
				{
					submitDrawData->DisplaySize.x != lastSubmitDisplaySize.x
					|| submitDrawData->DisplaySize.y != lastSubmitDisplaySize.y
					|| submitDrawData->DisplayPos.x != lastSubmitDisplayPos.x
					|| submitDrawData->DisplayPos.y != lastSubmitDisplayPos.y
				};

				lastSubmitDisplayPos.x = submitDrawData->DisplayPos.x;
				lastSubmitDisplayPos.y = submitDrawData->DisplayPos.y;

				lastSubmitDisplaySize.x = submitDrawData->DisplaySize.x;
				lastSubmitDisplaySize.y = submitDrawData->DisplaySize.y;
		
				return !haveValuesChanged;
		
			}

		void UiRenderer::CreateDescriptorBlockForUiResources()
		{			
			auto &viewFactory{ mediator->Renderer().GetViewFactory() };
			viewFactory.DeclareNewDescriptorBlock(uiSignature, 2, 0);
			viewFactory.CreateConstantBufferView(uiConstantBuffer, 1, constantBufferSizeInBytes);
			viewFactory.CreateShaderResourceView(uiFontTexture, 1, &FormatTargets::R8G8B8A8_UNorm, 1, 0);
			
			uiDescriptors = { &mediator->Renderer(), viewFactory.FinalizeDescriptorBlock() };
		
		}

		void UiRenderer::SubmitUiRendererWork()
		{
			auto &cmdFactory{ mediator->CommandFactory() };
			auto uiCommand{ MakeUnique<Commands::CompositeCommand>() };
						
			uiCommand->Add(cmdFactory.SetSignatureGraphics(uiSignature));
			uiCommand->Add(cmdFactory.SetPipelineState(uiPso));
			uiCommand->Add(cmdFactory.SetDescriptorBlockViewsGraphics(uiDescriptors));
			uiCommand->Add(cmdFactory.SetVertexBuffer(uiVertexIndexBuffer, 0, submitDrawData->TotalVtxCount, sizeof(ImDrawVert)));
			uiCommand->Add(cmdFactory.SetIndexBuffer
			(
				uiVertexIndexBuffer,
				vertexIndexBufferSizeInBytes,
				submitDrawData->TotalIdxCount,
				sizeof(ImDrawIdx),
				&FormatTargets::R16_Uint
			));
		
			RecordDrawDataDrawLists(*uiCommand, cmdFactory);

			mediator->Renderer().SubmitCommand(std::move(uiCommand));
		
		}

			void UiRenderer::RecordDrawDataDrawLists(Commands::CompositeCommand &targetCommand, Commands::CommandFactory &cmdFactory)
			{					
				drawRecordVertexOffset = 0;
				drawRecordIndexOffset = 0;

				for(unsigned drawListIndex{ 0 }; drawListIndex < submitDrawData->CmdListsCount; ++drawListIndex)
				{
					const auto &drawList{ *submitDrawData->CmdLists[drawListIndex] };
					RecordDrawListDrawCommands(drawList, targetCommand, cmdFactory);
				}
		
			}

				void UiRenderer::RecordDrawListDrawCommands
				(
					const ImDrawList &drawList,
					Commands::CompositeCommand &targetCommand,
					Commands::CommandFactory &cmdFactory
				)
				{
					const auto &clipOffset{ submitDrawData->DisplayPos };
					for(unsigned drawCommandIndex{ 0 }; drawCommandIndex < drawList.CmdBuffer.Size; ++drawCommandIndex)
					{
						const auto &drawCommand{ drawList.CmdBuffer[drawCommandIndex] };
						if(drawCommand.UserCallback != nullptr)
						{
							throw;
						}

						const auto topLeftX{ drawCommand.ClipRect.x - clipOffset.x };
						const auto topLeftY{ drawCommand.ClipRect.y - clipOffset.y };
						targetCommand.Add(cmdFactory.SetScissorRect
						(
							topLeftX,
							topLeftY,
							drawCommand.ClipRect.z - clipOffset.x - topLeftX,
							drawCommand.ClipRect.w - clipOffset.y - topLeftY 
						));

						targetCommand.Add(cmdFactory.DrawIndexedInstanced
						(
							1,
							drawCommand.ElemCount,
							drawCommand.IdxOffset + drawRecordIndexOffset,
							drawCommand.VtxOffset + drawRecordVertexOffset
						));				
					}
					drawRecordIndexOffset += drawList.IdxBuffer.Size;
					drawRecordVertexOffset += drawList.VtxBuffer.Size;
		
				}

	
}
