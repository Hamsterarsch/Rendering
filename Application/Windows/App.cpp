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
				vertex vertices[4]{ {-0.75, -0.75, 0 }, {0.75, -0.75, 0}, {0.75, 0.75, 0}, { -0.75, 0.75, 0} };
				unsigned indices[6]{ 0,1,2, 2,3,0 };
			} meshdata;
			meshSize = sizeof meshdata;
			meshBytesToIndices = sizeof meshdata.vertices;

			meshHandle = renderer->MakeBuffer(&meshdata, sizeof(meshdata));
						

			std::ifstream shaderFile{Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Plain.vs"), std::ios_base::in | std::ios_base::ate};
			Renderer::SerializeContainer vs{};
			{
			const auto charCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto shader{ std::make_unique<char[]>(charCount) };
			shaderFile.read( shader.get(), charCount);
						
			renderer->CompileVertexShader(shader.get(), charCount, &vs);
			
			shaderFile.close();
			}

			Renderer::SerializeContainer ps{};
			{
			shaderFile.open(Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Plain.ps"), std::ios_base::in | std::ios_base::ate);
				
			const auto psCharCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto pshader{ std::make_unique<char[]>(psCharCount) };
			shaderFile.read( pshader.get(), psCharCount);
						
			renderer->CompilePixelShader(pshader.get(), psCharCount, &ps);

			shaderFile.close();
			}
			
			Renderer::SerializeContainer root{};
			renderer->SerializeRootSignature(0,0,0,0, &root);

			rootHandle = renderer->MakeRootSignature(root.GetData());

			{
			Renderer::ShaderList shaderList{};
			shaderList.vs.data = vs.GetData();
			shaderList.vs.sizeInBytes = vs.GetSize();

			shaderList.ps.data = ps.GetData();
			shaderList.ps.sizeInBytes = ps.GetSize();

			psoHandle = renderer->MakePso(Renderer::PipelineTypes::Opaque, Renderer::VertexLayoutTypes::PositionOnly, shaderList, rootHandle);
			}


			Renderer::SerializeContainer vsm{};
			{
			shaderFile.open(Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/PlainMinstance.vs"), std::ios_base::in | std::ios_base::ate);
				
			const auto charCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto pshader{ std::make_unique<char[]>(charCount) };
			shaderFile.read( pshader.get(), charCount);
						
			Renderer::SerializeContainer ps{};
			renderer->CompileVertexShader(pshader.get(), charCount, &vsm);

			shaderFile.close();
			}
			
			{
				Renderer::ShaderList shaderList{};
				shaderList.vs.data = vsm.GetData();
				shaderList.vs.sizeInBytes = vsm.GetSize();

				shaderList.ps.data = ps.GetData();
				shaderList.ps.sizeInBytes = ps.GetSize();

				minstancePsoHandle = renderer->MakePso(Renderer::PipelineTypes::Opaque, Renderer::VertexLayoutTypes::PositionOnly, shaderList, rootHandle);
			}

			renderer->SetCamera(0, 0, -5, 0, 0, 0);
		
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

			renderer->RenderMesh(rootHandle, minstancePsoHandle, meshHandle, meshSize, meshBytesToIndices, transformBufferHandle, 2);			
			renderer->DispatchFrame();
			
		}

	
}
