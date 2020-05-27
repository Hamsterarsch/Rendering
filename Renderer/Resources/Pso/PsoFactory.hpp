#pragma once
#include <d3d12.h>
#include "DxPtrTypes.hpp"
#include "Resources/Pso/ShaderList.hpp"

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
		class BlendSettingsImpl;
		class DepthStencilSettingsImpl;

		class PsoFactory
		{
			private: RHA::DX12::DeviceResources *resources;

			private: BlendSettingsImpl *blendSettings;
			
			private: DepthStencilSettingsImpl *dsSettings;
			
			private: static constexpr DXGI_FORMAT rtvFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };

			private: static constexpr DXGI_FORMAT dsvFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

			private: static constexpr bool isFrontFaceCountClockwise{ true };



			public:	PsoFactory(RHA::DX12::DeviceResources *resources, DepthStencilSettingsImpl &dsSettings, BlendSettingsImpl &blendSettings);

						
			public: DxPtr<ID3D12PipelineState> MakePso(const ShaderList &shaders, ID3D12RootSignature *signature, PipelineTypes type, const D3D12_INPUT_LAYOUT_DESC &layout, D3D12_PRIMITIVE_TOPOLOGY_TYPE topology);

				private: D3D12_GRAPHICS_PIPELINE_STATE_DESC MakePipelineBase(PipelineTypes type) const;
			
				private: D3D12_GRAPHICS_PIPELINE_STATE_DESC MakeOpaquePipelineBase() const;

				private: static D3D12_SHADER_BYTECODE ConvertBlobToBytecode(const Blob &blob);

				private: static void CheckPipelineCreation(HRESULT result);

			public: DxPtr<ID3D12PipelineState> MakePso(const Blob &compiledComputeShader, ID3D12RootSignature *signature);
						
		};


	}

	
}
