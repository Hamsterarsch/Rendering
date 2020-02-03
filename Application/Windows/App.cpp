#include "Types/Dimensions2D.hpp"
#include "DX12/Facade.hpp"
#include "Windows/App.hpp"

#include "Shared/Filesystem/Conversions.hpp"
#include <fstream>


namespace Windows
{
	App::App() :
		window{ {1920, 1080}, L"Window", L"UniqueClassName" },
		renderer{ window.GetHandle() }
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

	class SerializeContainer : public Renderer::DX12::SerializationHook
	{
		public: const unsigned char *GetData() { return saved.get(); }

		public: size_t GetSize() const { return size; }
		
		UniquePtr<unsigned char[]> saved;
		size_t currentWriteOffset{ 0 };
		size_t size{ 0 };
		
		public:	void WriteToBlock(const void *data, size_t sizeInBytes) override
		{
			auto *bytePtr{ reinterpret_cast<const unsigned char *>(data) };
			memcpy(saved.get() + currentWriteOffset, data, sizeInBytes);

			currentWriteOffset += sizeInBytes;
			
		}

		protected: void OnBeginBlock() override
		{
			size = GetBlockSize();
			saved = std::make_unique<unsigned char[]>(size);
			
		}
		
	};

		
		void App::Initialize()
		{
			struct
			{
				vertex vertices[3]{ {-0.75, -0.75, 0 }, {0.75, -0.75, 0}, {0, 0.75, 0} };
				unsigned indices[3]{ 0,1,2 };
			} meshdata;
			meshSize = sizeof meshdata;
			meshBytesToIndices = sizeof meshdata.vertices;

			meshHandle = renderer.MakeBuffer(&meshdata, sizeof(meshdata));
						

			std::ifstream shaderFile{Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Plain.vs"), std::ios_base::in | std::ios_base::ate};
			SerializeContainer vs{};
			{
			const auto charCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto shader{ std::make_unique<char[]>(charCount) };
			shaderFile.read( shader.get(), charCount);
						
			renderer.CompileVertexShader(shader.get(), charCount, &vs);


			shaderFile.close();
			}
			
			shaderFile.open(Filesystem::Conversions::MakeExeRelative(L"../Content/Shaders/Plain.ps"), std::ios_base::in | std::ios_base::ate);
				
			const auto psCharCount{ shaderFile.tellg() };
			shaderFile.seekg(0);

			auto pshader{ std::make_unique<char[]>(psCharCount) };
			shaderFile.read( pshader.get(), psCharCount);
						
			SerializeContainer ps{};
			renderer.CompilePixelShader(pshader.get(), psCharCount, &ps);

			
			SerializeContainer root{};
			renderer.SerializeRootSignature(0,0,0,0, &root);

			rootHandle = renderer.MakeRootSignature(root.GetData());


			Renderer::ShaderList shaderList{};
			shaderList.vs.data = vs.GetData();
			shaderList.vs.sizeInBytes = vs.GetSize();

			shaderList.ps.data = ps.GetData();
			shaderList.ps.sizeInBytes = ps.GetSize();

			
			psoHandle = renderer.MakePso(Renderer::PipelineTypes::Opaque, Renderer::VertexLayoutTypes::PositionOnly, shaderList, rootHandle);
			
		
		}

		void App::Update()
		{
			if(renderer.ResourceMustBeRemade(meshHandle))
			{
				throw;
			}

			if(renderer.ResourceMustBeRemade(psoHandle))
			{
				throw;
			}

			if(renderer.ResourceMustBeRemade(rootHandle))
			{
				throw;
			}

			renderer.RenderMesh(rootHandle, psoHandle, meshHandle, meshSize, meshBytesToIndices);
			renderer.DispatchFrame();
			
		}

	
}