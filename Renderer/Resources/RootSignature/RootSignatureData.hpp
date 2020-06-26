#pragma once
#include "DxPtrTypes.hpp"
#include "Resources/RootSignature/TableLayout.hpp"
#include <vector>


namespace Renderer::DX12
{
	struct RootSignatureData
	{
		DxPtr<ID3D12RootSignature> signature;
		size_t samplerAmount;
		std::vector<TableLayout> layouts;

		RootSignatureData(const D3D12_ROOT_SIGNATURE_DESC1 &signatureDesc) :
			signature{ nullptr },
			samplerAmount{ 0 }				
		{
			for(unsigned parameterIndex{ 0 }; parameterIndex < signatureDesc.NumParameters; ++parameterIndex)
			{
				if(signatureDesc.pParameters[parameterIndex].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
				{
					layouts.emplace_back(&signatureDesc.pParameters[parameterIndex].DescriptorTable);
				}
			}
							
		}

	};

	
}
