#pragma once
#include <d3d12.h>
#include <vector>
#include "DxPtrTypes.hpp"
#include "Resources/RootSignature/RootSignatureData.hpp"
#include "StateSettings/SamplerSpec.hpp"


namespace RHA::DX12{ class DeviceResources; }


namespace Renderer::DX12
{
	class RootSignatureSettingsImpl;


	class RootSignatureFactory
	{
		private: RHA::DX12::DeviceResources *resources;

		private: const RootSignatureSettingsImpl *settings;
		
		private: size_t offsetInDescriptorTable;



		public: explicit RootSignatureFactory(RHA::DX12::DeviceResources *resources, const RootSignatureSettingsImpl &settings);			

		
		public: DxPtr<ID3DBlob> SerializeRootSignature
		(			
			const SamplerSpec *staticSamplers,
			unsigned numStaticSamplers
		);

			private: void PushBackRangeIfNecessary(std::vector<D3D12_DESCRIPTOR_RANGE1> &container,  unsigned descriptorAmount, D3D12_DESCRIPTOR_RANGE_TYPE type);
			
				private: D3D12_DESCRIPTOR_RANGE1 MakeRangeDesc(unsigned descriptorAmount, D3D12_DESCRIPTOR_RANGE_TYPE type) const;

			private: static void CheckSerialization(HRESULT result);

			private: static void CheckRootSignatureCreation(HRESULT result);

		
		public: RootSignatureData MakeRootSignature(const void *serializedSignature, size_t sizeInBytes, size_t samplerCount);
								
	};

		
}
