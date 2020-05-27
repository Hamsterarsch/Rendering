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



namespace Windows
{
	App::App() :
		window{ {1920, 1080}, true, L"Window", L"UniqueClassName" },
		renderer{ Renderer::MakeRenderer(window.GetHandle()) }
	{
		//Initialize();

		
		::App::Rendering::RendererMediator mediator{{renderer.get(), renderer->MakeWindowsWindowSurface(window.GetHandle())}, *renderer, {mediator, {1,1}} };
		auto &blendSettings{ renderer->GetBlendSettings() };
		auto &depthSettings{ renderer->GetDepthStencilSettings() };


		//dearimgui render setup
		depthSettings.SetEnableDepth(false);

		
		blendSettings.SetEnableBlend(true);
		
		blendSettings.SetBlendSrcAlpha(&Renderer::BlendSettings::TargetSrc);
		blendSettings.SetBlendInverseSrcAlpha(&Renderer::BlendSettings::TargetDst);
		
		blendSettings.SetBlendInverseSrcAlpha(&Renderer::BlendSettings::TargetSrcAlpha);
		blendSettings.SetBlendZero(&Renderer::BlendSettings::TargetDstAlpha);

		blendSettings.SetBlendOpAdd(&Renderer::BlendSettings::TargetBlendOpColor);
		blendSettings.SetBlendOpAdd(&Renderer::BlendSettings::TargetBlendOpAlpha);

		renderer->MakePso()
		
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
				DispatchMessage(&msg);
			}
			else
			{
				mediator.SubmitFrame();
				Update();
			}
		}
		
	}

	struct vertex
	{
		float x, y, z;
	};

		
		void App::Initialize()
		{
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


			renderer->SetCamera(0, 0, -11, 0, 0, 0);
			
			renderer->MakeLight({0, 0, -1}, {0, 0, 0}, {1, 4, 4}, 3);
			renderer->MakeLight({4, 0, -2}, {0, 0, 0}, {6, 0, 0},  5);			
		
		}

	
	   	
		void App::Update()
		{			

			
		}

	
}
