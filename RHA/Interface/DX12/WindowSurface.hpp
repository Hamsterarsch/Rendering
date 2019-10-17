#pragma once
#include "Shared/InterfaceHelpers.hpp"


struct ID3D12GraphicsCommandList;


namespace RHA
{
	namespace DX12
	{
		class WindowSurface
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(WindowSurface)


			public: virtual unsigned GetBufferCount() const = 0;
			
			public: virtual void ClearBuffer(DxPtr<ID3D12GraphicsCommandList> list, unsigned index) = 0;

			public: virtual void Present() = 0;
							
		};

		
	}

	
}
