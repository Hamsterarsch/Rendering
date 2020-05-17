#include "Types/Dimensions2D.hpp"
#include "DX12/Facade.hpp"
#include "Windows/App.hpp"

#include "Shared/Filesystem/Conversions.hpp"
#include <fstream>


#include "ThirdParty/glm/mat4x4.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Resources/SerializationContainer.hpp"

namespace Windows
{
	App::App() :
		window{ {1920, 1080}, true, L"Window", L"UniqueClassName" },
		renderer{ Renderer::MakeRenderer(window.GetHandle()) }
	{
		Initialize();

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
				vertex vertexData[8]{ {-0.75, -0.75, 0 }, {0,0,1}, {0.75, -0.75, 0}, {0,0,1}, {0.75, 0.75, 0}, {0,0,1}, { -0.75, 0.75, 0}, {0,0,1} };
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
							
				Renderer::SerializeContainer ps{};
				renderer->CompilePixelShader(pshader.get(), charCount, &ps);
								
			}


			Renderer::SerializeContainer vs{};
			{
				std::ifstream shaderFile{Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/LightingInstanced.vs"), std::ios_base::in | std::ios_base::ate };
					
				const auto charCount{ shaderFile.tellg() };
				shaderFile.seekg(0);

				auto pshader{ std::make_unique<char[]>(charCount) };
				shaderFile.read( pshader.get(), charCount);
							
				Renderer::SerializeContainer ps{};
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
			renderer->MakeLight(0,0,0,0,0,0);
			renderer->MakeLight(8,0,0,0,0,0);
			renderer->MakeLight(-11, 0, 0, 0, 0, 0);
		
		}

	
	   	
		void App::Update()
		{			
			if(renderer->IsBusy())
			{
				return;
				
			}
			
			const auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.f;
			const auto rot = rotate(glm::identity<glm::mat4>(), glm::radians(currentTime * 7), {0,0,1});
			std::vector<glm::mat4> transformData
			{
				rot,
				translate(glm::identity<glm::mat4>(), {4, 0, 0}) * rot
			};						
			const Renderer::DX12::HandleWrapper transformBufferHandle{ renderer.get(), renderer->MakeBuffer(transformData.data(), sizeof glm::mat4 * transformData.size()) };
			
			if(renderer->ResourceMustBeRemade(meshHandle))
			{
				throw;
			}

			if(renderer->ResourceMustBeRemade(psoHandle))
			{
				throw;
			}

			if(renderer->ResourceMustBeRemade(rootHandle))
			{
				throw;
			}

			renderer->RenderMesh(rootHandle, psoOpaqueShadedWithInstanceSupport, meshHandle, meshSize, meshBytesToIndices, transformBufferHandle, 2);			
			renderer->DispatchFrame();
			
		}

	
}
