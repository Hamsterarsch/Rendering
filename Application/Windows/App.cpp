#include "Types/Dimensions2D.hpp"
#include "DX12/Facade.hpp"
#include "Windows/App.hpp"


namespace Windows
{
	App::App() :
		window{ {1920, 1080}, L"Window", L"UniqueClassName" }
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

		void App::Initialize()
		{
			auto resc
			{
				RHA::DX12::Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_12_0, true)
			};


			auto alloc
			{
				RHA::DX12::Facade::MakeCmdAllocator(resc.get(), D3D12_COMMAND_LIST_TYPE_DIRECT)
			};

			auto cmdList
			{
				alloc->AllocateList()
			};

			auto graList
			{
				cmdList->AsGraphicsList()
			};

			auto result
			{
				graList->Close()
			};


			auto queue
			{
				RHA::DX12::Facade::MakeQueue(resc.get(), D3D12_COMMAND_LIST_TYPE_DIRECT)
			};

			auto surface
			{
				RHA::DX12::Facade::MakeWindowSurface(resc.get(), queue.get(), window.GetHandle())
			};

			queue->SubmitCommandList(cmdList.get());

		}

		void App::Update()
		{
		}

}