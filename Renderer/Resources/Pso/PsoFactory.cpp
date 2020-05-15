#include "DX12/DeviceResources.hpp"
#include "Resources/Pso/PipelineTypes.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Resources/Pso/PsoFactory.hpp"
#include "Resources/Pso/DepthStencilFactory.hpp"


namespace Renderer::DX12
{
	PsoFactory::PsoFactory(RHA::DX12::DeviceResources *resources, DepthStencilFactory &dsFactory) :
		resources{ resources },
		dsFactory{ dsFactory }
	{			
	}
	
	DxPtr<ID3D12PipelineState> PsoFactory::MakePso
	(
		const ShaderList &shaders,
		ID3D12RootSignature *signature,
		const PipelineTypes type,
		const D3D12_INPUT_LAYOUT_DESC &layout,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topology
	)
	{
		auto desc{ MakePipelineBase(type) };
		
		desc.InputLayout = layout;
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

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoFactory::MakePipelineBase(const PipelineTypes type) const
		{
			switch(type)
			{
			case PipelineTypes::Opaque: return MakeOpaquePipelineBase();
			default: throw Exception::Exception{ "Missing base pipeline specification for dx12 pipeline type" };
			}
			
		}

			D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoFactory::MakeOpaquePipelineBase() const
			{
				D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
				
				desc.DepthStencilState = dsFactory.MakeDepthStencilDesc();
							
				desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
				desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
				desc.RasterizerState.FrontCounterClockwise = isFrontFaceCountClockwise;
				desc.RasterizerState.DepthClipEnable = true;

				desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				
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
