#include "DX12/DeviceResources.hpp"
#include "Resources/RootSignature/RootSignatureFactory.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "StateSettings/AddressingTargetsImpl.hpp"
#include "StateSettings/FilterTargetsImpl.hpp"
#include "StateSettings/RootSignatureSettingsImpl.hpp"


namespace Renderer::DX12
{

	RootSignatureFactory::RootSignatureFactory(RHA::DX12::DeviceResources *resources, const RootSignatureSettingsImpl &settings) :
		resources{ resources },
		settings{ &settings }
	{
	}

	
	
	DxPtr<ID3DBlob> RootSignatureFactory::SerializeRootSignature
	(		
		const SamplerSpec *staticSamplers,
		const unsigned numStaticSamplers
	)
	{
		std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplerDescs(numStaticSamplers);
		for(unsigned staticSamplerIndex{ 0 }; staticSamplerIndex < numStaticSamplers; ++staticSamplerIndex)
		{
			auto &inDesc{ staticSamplers[staticSamplerIndex] };
			
			D3D12_STATIC_SAMPLER_DESC desc{};
			desc.AddressU = *reinterpret_cast<const D3D12_TEXTURE_ADDRESS_MODE *>((AddressingTargetsImpl::Get().*inDesc.addressU)());
			desc.AddressV = *reinterpret_cast<const D3D12_TEXTURE_ADDRESS_MODE *>((AddressingTargetsImpl::Get().*inDesc.addressV)());
			desc.AddressW = *reinterpret_cast<const D3D12_TEXTURE_ADDRESS_MODE *>((AddressingTargetsImpl::Get().*inDesc.addressW)());
			desc.Filter = *reinterpret_cast<const D3D12_FILTER *>((FilterTargetsImpl::Get().*inDesc.filter)());
			
			desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			
			desc.ShaderRegister = staticSamplerIndex;

			staticSamplerDescs.at(staticSamplerIndex) = std::move(desc);			
		}

		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc{};
		rootDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		
		rootDesc.Desc_1_1.pStaticSamplers = staticSamplerDescs.data();
		rootDesc.Desc_1_1.NumStaticSamplers = staticSamplerDescs.size();
				
		rootDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootDesc.Desc_1_1.NumParameters = settings->GetParameters().size();
		rootDesc.Desc_1_1.pParameters = settings->GetParameters().data();
		
		
		DxPtr<ID3DBlob> rootBlob, errorBlob;
		{
			const auto result{ D3D12SerializeVersionedRootSignature(&rootDesc, &rootBlob, &errorBlob) };
			if(errorBlob)
			{
				auto *errorString{ reinterpret_cast<const char *>(errorBlob->GetBufferPointer()) };
				int e = 2;
			}
			
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
				offsetInDescriptorTable += descriptorAmount;
			}
			
		}

			D3D12_DESCRIPTOR_RANGE1 RootSignatureFactory::MakeRangeDesc
			(
				const unsigned descriptorAmount,
				const D3D12_DESCRIPTOR_RANGE_TYPE type
			) const
			{
				D3D12_DESCRIPTOR_RANGE1 rangeDesc{};
				rangeDesc.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
				rangeDesc.OffsetInDescriptorsFromTableStart = offsetInDescriptorTable;										
				rangeDesc.BaseShaderRegister = type == D3D12_DESCRIPTOR_RANGE_TYPE_UAV ? 1 : 0; //todo: in pixel shader register u0 is reserved for output
				rangeDesc.BaseShaderRegister = type == D3D12_DESCRIPTOR_RANGE_TYPE_CBV ? 2 : rangeDesc.BaseShaderRegister;
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
		const size_t sizeInBytes,
		const size_t samplerCount
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

		out.samplerAmount = samplerCount;			
		return out;
		
	}

		
}
