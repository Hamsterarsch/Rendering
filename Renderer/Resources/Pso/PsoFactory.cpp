#include "DX12/DeviceResources.hpp"
#include "Resources/Pso/PipelineTypes.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Resources/Pso/PsoFactory.hpp"


namespace Renderer
{
	namespace DX12
	{		
		DxPtr<ID3D12PipelineState> PsoFactory::MakePso
		(
			const ShaderInfo &shaders,
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
			const auto result
			{
				resources->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline))
			};
			CheckPipelineCreation(result);

			return pipeline;
			
		}

			D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoFactory::MakePipelineBase(const PipelineTypes type)
			{
				switch(type)
				{
				case PipelineTypes::Opaque: return MakeOpaquePipelineBase();
				default: throw Exception::Exception{ "Missing base pipeline specification for dx12 pipeline type" };
				}
				
			}

				constexpr D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoFactory::MakeOpaquePipelineBase()
				{
					D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
					
					desc.DepthStencilState.StencilEnable = false;

					desc.DepthStencilState.DepthEnable = true;
					desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
					desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;			
					desc.DepthStencilState.BackFace = desc.DepthStencilState.FrontFace = MakeDefaultStencilDesc();
								
					desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
					desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
					desc.RasterizerState.FrontCounterClockwise = isFrontFaceCountClockwise;
					desc.RasterizerState.DepthClipEnable = true;

					desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
					
					desc.NumRenderTargets = 1;
					desc.RTVFormats[0] = rtvFormat;
					desc.DSVFormat[0] = dsvFormat;
					
					desc.SampleDesc.Count = 1;
					desc.SampleDesc.Quality = 0;
					desc.SampleMask = UINT_MAX;
					
					return desc;
					
				}

					constexpr D3D12_DEPTH_STENCILOP_DESC PsoFactory::MakeDefaultStencilDesc()
					{
						return { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
						
					}

			D3D12_SHADER_BYTECODE PsoFactory::ConvertBlobToBytecode(ID3DBlob *const blob)
			{
				if(blob == nullptr)
				{
					return {};
				}

				return { blob->GetBufferPointer(), blob->GetBufferSize() };
				
			}
					   		
			void PsoFactory::CheckPipelineCreation(const HRESULT result)
			{
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create dx12 pipeline state" };
				}
			
			}

		
	}


	
}
