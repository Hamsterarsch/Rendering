#include "DX12/DeviceResources.hpp"
#include "Resources/Pso/PipelineTypes.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Resources/Pso/PsoFactory.hpp"
#include "StateSettings/BlendSettingsImpl.hpp"
#include "StateSettings/DepthStencilSettingsImpl.hpp"
#include "StateSettings/RasterizerSettingsImpl.hpp"
#include "StateSettings/VertexLayoutSettingsImpl.hpp"


namespace Renderer::DX12
{
	PsoFactory::PsoFactory
	(
		RHA::DX12::DeviceResources *resources,
		DepthStencilSettingsImpl &dsSettings,
		BlendSettingsImpl &blendSettings,
		RasterizerSettingsImpl &rasterizerSettings,
		VertexLayoutSettingsImpl &vertexLayoutSettings
	)	:
		resources{ resources },
		blendSettings{ &blendSettings },
		dsSettings{ &dsSettings },
		rasterizerSettings{ &rasterizerSettings },
		vertexLayoutSettings{ &vertexLayoutSettings }
	{			
	}


	
	DxPtr<ID3D12PipelineState> PsoFactory::MakePso
	(
		const ShaderList &shaders,
		ID3D12RootSignature *signature,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topology
	)
	{
		auto desc{ MakePipelineDesc() };		
		desc.PrimitiveTopologyType = topology;
					
		desc.VS = ConvertBlobToBytecode(shaders.vs);
		desc.HS = ConvertBlobToBytecode(shaders.hs);
		desc.GS = ConvertBlobToBytecode(shaders.gs);
		desc.DS = ConvertBlobToBytecode(shaders.ds); 
		desc.PS = ConvertBlobToBytecode(shaders.ps);

		desc.pRootSignature = signature;
		
		DxPtr<ID3D12PipelineState> pipeline;

		try
		{
			const auto result
			{
				resources->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline))
			};
			CheckPipelineCreation(result);
		}
		catch(...)
		{
			CheckPipelineCreation(E_FAIL);
		}

		return pipeline;
		
	}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoFactory::MakePipelineDesc() const
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
			
			desc.DepthStencilState = dsSettings->GetDepthStencilDesc();
			desc.BlendState.RenderTarget[0] = blendSettings->GetBlendDesc();
			desc.RasterizerState = rasterizerSettings->GetRasterizerDesc();
			desc.InputLayout = vertexLayoutSettings->GetLayoutDesc();
		
			desc.NumRenderTargets = 1;
			desc.RTVFormats[0] = rtvFormat;
			desc.DSVFormat = dsvFormat;
			
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.SampleMask = UINT_MAX;
			
			return desc;
			
		}



		D3D12_SHADER_BYTECODE PsoFactory::ConvertBlobToBytecode(const Blob &blob)
		{
			if(blob.data == nullptr)
			{
				return {};
			}
			
			return { blob.data, blob.sizeInBytes };
			
		}
				   		
		void PsoFactory::CheckPipelineCreation(const HRESULT result)
		{
			if(FAILED(result))
			{
				throw Exception::CreationFailed{ "Could not create dx12 pipeline state" };
			}
		
		}

		
	
	DxPtr<ID3D12PipelineState> PsoFactory::MakePso(const Blob &compiledComputeShader, ID3D12RootSignature *signature)
	{
		D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
		desc.CS = ConvertBlobToBytecode(compiledComputeShader);
		desc.pRootSignature = signature;

		DxPtr<ID3D12PipelineState> pipeline;
		const auto result
		{
			resources->GetDevice()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipeline))
		};
		CheckPipelineCreation(result);

		return pipeline;
		
	}

	
}
