#pragma once
#include <d3d12.h>
#include <vector>
#include "DxPtrTypes.hpp"
#include "Resources/RootSignature/TableLayout.hpp"

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
		struct RootSignatureData
		{
			DxPtr<ID3D12RootSignature> signature;
			TableLayout layout;
		};
		
		class RootSignatureFactory
		{
			private: RHA::DX12::DeviceResources *resources;

			private: DxPtr<ID3DBlob> rootBlob;



			public: explicit RootSignatureFactory(RHA::DX12::DeviceResources *resources);			

			public: inline const ID3DBlob *GetLastRootSignatureBlob() const { return rootBlob.Get(); }
			
			
			public: RootSignatureData MakeRootSignature(unsigned cbvAmount, unsigned srvAmount, unsigned uavAmount);
			
				private: RootSignatureData MakeRootSignatureImpl(unsigned cbvAmount, unsigned srvAmount, unsigned uavAmount, unsigned samplerAmount);

					private: void PushBackRangeIfNecessary(std::vector<D3D12_DESCRIPTOR_RANGE1> &container,  unsigned descriptorAmount, D3D12_DESCRIPTOR_RANGE_TYPE type);
				
						private: static D3D12_DESCRIPTOR_RANGE1 MakeRangeDesc(unsigned descriptorAmount, D3D12_DESCRIPTOR_RANGE_TYPE type);

					private: static void CheckSerialization(HRESULT result);

					private: static void CheckRootSignatureCreation(HRESULT result);
			
			
			public: RootSignatureData MakeRootSignature(unsigned samplerAmount);
						
		};

		
	}
	
	
}
