#include "DX12/DeviceResources.hpp"
#include "Resources/RootSignature/RootSignatureFactory.hpp"
#include "Shared/Exception/CreationFailedException.hpp"


namespace Renderer
{
	namespace DX12
	{
		RootSignatureFactory::RootSignatureFactory(RHA::DX12::DeviceResources *resources) :
			resources{ resources }
		{
		}


		
		
		DxPtr<ID3DBlob> RootSignatureFactory::SerializeRootSignature
		(
			const unsigned cbvAmount, 
			const unsigned srvAmount, 
			const unsigned uavAmount, 
			const unsigned samplerAmount
		)
		{
			D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc{};
			rootDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;

			std::vector<D3D12_ROOT_PARAMETER1> parameters{};
		
			std::vector<D3D12_DESCRIPTOR_RANGE1> ranges{};
			PushBackRangeIfNecessary(ranges, srvAmount, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
			PushBackRangeIfNecessary(ranges, uavAmount, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
			PushBackRangeIfNecessary(ranges, cbvAmount, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
										
			D3D12_ROOT_PARAMETER1 viewParamDesc{};
			viewParamDesc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			viewParamDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			viewParamDesc.DescriptorTable.pDescriptorRanges = ranges.data();
			viewParamDesc.DescriptorTable.NumDescriptorRanges = ranges.size();

			if(viewParamDesc.DescriptorTable.NumDescriptorRanges > 0)
			{
				parameters.emplace_back(viewParamDesc);				
			}

			D3D12_DESCRIPTOR_RANGE1 samplerRange{};
			if(samplerAmount > 0)
			{
				samplerRange.BaseShaderRegister = 0;
				samplerRange.NumDescriptors = samplerAmount;
				samplerRange.OffsetInDescriptorsFromTableStart = 0;
				samplerRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
				samplerRange.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
				
				D3D12_ROOT_PARAMETER1 samplerParamDesc{};					
				samplerParamDesc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				samplerParamDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				samplerParamDesc.DescriptorTable.pDescriptorRanges = &samplerRange;
				samplerParamDesc.DescriptorTable.NumDescriptorRanges = 1;
				parameters.emplace_back(samplerParamDesc);					
			}				

			rootDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			rootDesc.Desc_1_1.NumParameters = parameters.size();
			rootDesc.Desc_1_1.pParameters = parameters.data();

			
			DxPtr<ID3DBlob> rootBlob, errorBlob;
			{
				const auto result{ D3D12SerializeVersionedRootSignature(&rootDesc, &rootBlob, &errorBlob) };
				CheckSerialization(result);
			}
					   
			return rootBlob;
			
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


		
		RootSignatureData RootSignatureFactory::MakeRootSignature
		(
			const void *serializedSignature,
			const size_t sizeInBytes
		)
		{
			DxPtr<ID3D12VersionedRootSignatureDeserializer> deserializer;
			{
				const auto result
				{
					D3D12CreateVersionedRootSignatureDeserializer(serializedSignature, sizeInBytes, IID_PPV_ARGS(&deserializer))
				};
				
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not make dx12 root signature from serialized blob" };
				}
			}
			
			RootSignatureData out{ deserializer->GetUnconvertedRootSignatureDesc()->Desc_1_1 };
			
			const auto result
			{
				resources->GetDevice()->CreateRootSignature
				(
					0,
					serializedSignature,
					sizeInBytes,
					IID_PPV_ARGS(&out.signature)
				)
			};
			CheckRootSignatureCreation(result);

			return out;
			
		}

		
	}

	
}
