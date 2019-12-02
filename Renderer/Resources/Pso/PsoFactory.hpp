#pragma once
#include <d3d12.h>
#include "DxPtrTypes.hpp"


namespace Renderer
{
	enum class PipelineTypes : unsigned char;
	
	namespace DX12
	{
		struct ShaderInfo
		{
			ID3DBlob *vertex, *tesselationControl, *tesselationEvaluation, *pixel;
			
		};
		
		class PsoFactory
		{
			DxPtr<ID3D12PipelineState> MakePso(const ShaderInfo &shaders, ID3D12RootSignature *signature, PipelineTypes type);
						
		};
		
		
	}

	
}
