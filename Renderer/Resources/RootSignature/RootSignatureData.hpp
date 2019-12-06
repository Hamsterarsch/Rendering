#pragma once
#include "DxPtrTypes.hpp"
#include "Resources/RootSignature/TableLayout.hpp"


namespace Renderer
{
	namespace DX12
	{
		struct RootSignatureData
		{
			DxPtr<ID3D12RootSignature> signature;
			TableLayout layout;
			size_t samplerAmount;

			RootSignatureData(const D3D12_ROOT_DESCRIPTOR_TABLE1 &viewTableLayout) :
				signature{ nullptr },
				layout{ viewTableLayout },
				samplerAmount{ 0 }
			{				
			}

		};

		
	}

	
}
