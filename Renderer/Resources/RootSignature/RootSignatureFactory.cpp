#include "DX12/DeviceResources.hpp"
#include "Resources/RootSignature/RootSignatureFactory.hpp"
#include "Shared/Exception/CreationFailedException.hpp"


namespace Renderer
{
	namespace DX12
	{
		RootSignatureFactory::RootSignatureFactory(RHA::DX12::DeviceResources *resources) :
			resources{ resources },
			rootBlob{ nullptr }
		{
		}


		
		RootSignatureData RootSignatureFactory::MakeRootSignature
		(
			const unsigned cbvAmount, 
			const unsigned srvAmount,
			const unsigned uavAmount
		)
		{
			return MakeRootSignatureImpl(cbvAmount, srvAmount, uavAmount, 0);
			
		}

			RootSignatureData RootSignatureFactory::MakeRootSignatureImpl
			(
				const unsigned cbvAmount, 
				const unsigned srvAmount, 
				const unsigned uavAmount, 
				const unsigned samplerAmount
			)
			{
				D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc{};
				rootDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
				
				std::vector<D3D12_DESCRIPTOR_RANGE1> ranges{};
				PushBackRangeIfNecessary(ranges, srvAmount, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
				PushBackRangeIfNecessary(ranges, uavAmount, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
				PushBackRangeIfNecessary(ranges, cbvAmount, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
				PushBackRangeIfNecessary(ranges, samplerAmount, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER);
							
				D3D12_ROOT_PARAMETER1 paramDesc{};
				paramDesc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				paramDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				paramDesc.DescriptorTable.pDescriptorRanges = ranges.data();
				paramDesc.DescriptorTable.NumDescriptorRanges = ranges.size();

				rootDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
				rootDesc.Desc_1_1.NumParameters = 1;
				rootDesc.Desc_1_1.pParameters = &paramDesc;

				DxPtr<ID3DBlob> errorBlob;
				{
					const auto result{ D3D12SerializeVersionedRootSignature(&rootDesc, &rootBlob, &errorBlob) };
					CheckSerialization(result);
				}

				RootSignatureData out{ nullptr, TableLayout(paramDesc.DescriptorTable) };
				const auto result
				{
					resources->GetDevice()->CreateRootSignature
					(
						0,
						rootBlob->GetBufferPointer(),
						rootBlob->GetBufferSize(),						
						IID_PPV_ARGS(&out.signature)
					)
				};
				CheckRootSignatureCreation(result);

				return out;			
				
			}

				void RootSignatureFactory::PushBackRangeIfNecessary
				(
					std::vector<D3D12_DESCRIPTOR_RANGE1> &container,
					const unsigned descriptorAmount,
					const D3D12_DESCRIPTOR_RANGE_TYPE type
				)
				{
					if(descriptorAmount > 0)
					{
						container.push_back(MakeRangeDesc(descriptorAmount, type));
					}
					
				}

					D3D12_DESCRIPTOR_RANGE1 RootSignatureFactory::MakeRangeDesc
					(
						const unsigned descriptorAmount,
						const D3D12_DESCRIPTOR_RANGE_TYPE type
					)
					{
						D3D12_DESCRIPTOR_RANGE1 rangeDesc{};
						rangeDesc.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
						rangeDesc.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
						rangeDesc.BaseShaderRegister = 0;
						rangeDesc.NumDescriptors = descriptorAmount;
						rangeDesc.RangeType = type;

						return rangeDesc;
						
					}

				void RootSignatureFactory::CheckSerialization(const HRESULT result)
				{
					if(FAILED(result))
					{
						throw Exception::Exception{ "Could not serialize root signature" };						
					}
			
				}

				void RootSignatureFactory::CheckRootSignatureCreation(const HRESULT result)
				{
					if(FAILED(result))
					{
						throw Exception::CreationFailed{ "Could not create dx12 root signature" };
					}
			
				}

		

		RootSignatureData RootSignatureFactory::MakeRootSignature(const unsigned samplerAmount)
		{
			return MakeRootSignatureImpl(0, 0, 0, samplerAmount);
			
		}

		
	}

	
}
