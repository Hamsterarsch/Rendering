#include "Window/Windows/Window.hpp"
#include "Types/Dimensions2D.hpp"
#include "DX12/Facade.hpp"
#include "DX12/DeviceResources.hpp"
#include "Application/App.hpp"

App::App()
{
	Windows::Window window{ {1920, 1080}, L"Window", L"UniqueClassName" };

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
