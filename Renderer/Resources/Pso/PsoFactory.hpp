#pragma once
#include <d3d12.h>
#include "DxPtrTypes.hpp"

namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
	}
}


namespace Renderer
{
	enum class PipelineTypes : unsigned char;
	
	namespace DX12
	{
		struct ShaderInfo
		{
			ID3DBlob *vs, *hs, *gs, *ds, *ps;
			
		};
		
		class PsoFactory
		{
			private: RHA::DX12::DeviceResources *resources;

			private: static constexpr DXGI_FORMAT rtvFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };

			private: static constexpr DXGI_FORMAT dsvFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

			private: static constexpr bool isFrontFaceCountClockwise{ true };



			public:	explicit PsoFactory(RHA::DX12::DeviceResources *resources);

						
			public: DxPtr<ID3D12PipelineState> MakePso(const ShaderInfo &shaders, ID3D12RootSignature *signature, PipelineTypes type, const D3D12_INPUT_LAYOUT_DESC &layout, D3D12_PRIMITIVE_TOPOLOGY_TYPE topology);

				private: static D3D12_GRAPHICS_PIPELINE_STATE_DESC MakePipelineBase(PipelineTypes type);
			
				private: static constexpr D3D12_GRAPHICS_PIPELINE_STATE_DESC MakeOpaquePipelineBase();

					private: static constexpr D3D12_DEPTH_STENCILOP_DESC MakeDefaultStencilDesc();
			
				private: static D3D12_SHADER_BYTECODE ConvertBlobToBytecode(ID3DBlob *blob);

				private: static void CheckPipelineCreation(HRESULT result);
						
		};


	}

	
}
