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
			size_t samplerAmount;
			TableLayout layout;

			RootSignatureData(const D3D12_ROOT_SIGNATURE_DESC1 &signatureDesc) :
				signature{ nullptr },
				samplerAmount{ 0 },
				layout
				{
					signatureDesc.NumParameters > 2 && signatureDesc.pParameters[2].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE
					? &signatureDesc.pParameters[2].DescriptorTable
					: nullptr
				}
			{				
			}

		};

		
	}

	
}
