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
		class RasterizerSettingsImpl;
		class VertexLayoutSettingsImpl;

		class PsoFactory
		{
			private: RHA::DX12::DeviceResources *resources;

			private: BlendSettingsImpl *blendSettings;
			
			private: DepthStencilSettingsImpl *dsSettings;

			private: RasterizerSettingsImpl *rasterizerSettings;

			private: VertexLayoutSettingsImpl *vertexLayoutSettings;
					
			private: static constexpr DXGI_FORMAT rtvFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };

			private: static constexpr DXGI_FORMAT dsvFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

			private: static constexpr bool isFrontFaceCountClockwise{ false };//changed for imgui tests



			public:	PsoFactory
			(
				RHA::DX12::DeviceResources *resources,
				DepthStencilSettingsImpl &dsSettings,
				BlendSettingsImpl &blendSettings,
				RasterizerSettingsImpl &rasterizerSettings,
				VertexLayoutSettingsImpl &vertexLayoutSettings
			);
	
						
			public: DxPtr<ID3D12PipelineState> MakePso(const ShaderList &shaders, ID3D12RootSignature *signature, D3D12_PRIMITIVE_TOPOLOGY_TYPE topology);

				private: D3D12_GRAPHICS_PIPELINE_STATE_DESC MakePipelineDesc() const;
			
				private: static D3D12_SHADER_BYTECODE ConvertBlobToBytecode(const Blob &blob);

				private: static void CheckPipelineCreation(HRESULT result);

			public: DxPtr<ID3D12PipelineState> MakePso(const Blob &compiledComputeShader, ID3D12RootSignature *signature);
						
		};


	}

	
}
