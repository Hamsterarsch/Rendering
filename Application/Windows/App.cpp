#include "Types/Dimensions2D.hpp"
#include "DX12/Facade.hpp"
#include "Windows/App.hpp"


namespace Windows
{
	App::App() :
		window{ {1920, 1080}, L"Window", L"UniqueClassName" },
		renderer{ 3 }
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
		}

		void App::Update()
		{
		}

	
}