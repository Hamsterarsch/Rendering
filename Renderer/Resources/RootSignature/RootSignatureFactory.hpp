#pragma once
#include <d3d12.h>
#include <vector>
#include "DxPtrTypes.hpp"
#include "Resources/RootSignature/RootSignatureData.hpp"

namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
	}
}


namespace Renderer
{
	namespace DX12
	{

		
		class RootSignatureFactory
		{
			private: RHA::DX12::DeviceResources *resources;



			public: explicit RootSignatureFactory(RHA::DX12::DeviceResources *resources);			

			
			public: DxPtr<ID3DBlob> SerializeRootSignature(unsigned cbvAmount, unsigned srvAmount, unsigned uavAmount, unsigned samplerAmount);

				private: void PushBackRangeIfNecessary(std::vector<D3D12_DESCRIPTOR_RANGE1> &container,  unsigned descriptorAmount, D3D12_DESCRIPTOR_RANGE_TYPE type);
				
					private: static D3D12_DESCRIPTOR_RANGE1 MakeRangeDesc(unsigned descriptorAmount, D3D12_DESCRIPTOR_RANGE_TYPE type);

				private: static void CheckSerialization(HRESULT result);

				private: static void CheckRootSignatureCreation(HRESULT result);

			
			public: RootSignatureData MakeRootSignature(const void *serializedSignature, size_t sizeInBytes, size_t samplerCount);
									
		};

		
	}
	
	
}
