#pragma once
#include "DxPtrTypes.hpp"
#include <d3d12.h>
#include "Shared/InterfaceHelpers.hpp"

namespace RHA
{
	namespace DX12
	{
		class ShaderFactory
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(ShaderFactory)



			public: virtual void SkipOptimization(bool value) = 0;

			public: virtual void AddDebugInfo(bool value) = 0;

			public: virtual void AllowHighOptimizationTimes(bool value) = 0;

			
			public: virtual DxPtr<ID3DBlob> MakeVertexShader(const wchar_t *filepath, const char *entrypoint) = 0;

			public: virtual DxPtr<ID3DBlob> MakePixelShader(const wchar_t *filepath, const char *entrypoint) = 0;
			
		};

		
	}
	
	
}

