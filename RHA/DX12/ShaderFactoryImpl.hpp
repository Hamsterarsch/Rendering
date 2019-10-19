#pragma once
#include <d3d12.h>
#include <string>
#include "DxPtrTypes.hpp"


namespace RHA
{
	namespace DX12
	{
		class ShaderFactoryImpl
		{
			std::string shaderModelSpec;
			
			bool insertDebugInfo;
			
			bool skipOptimization;
			
			bool useHighOptimization;


			
			public: ShaderFactoryImpl(unsigned char shaderModelMajor, unsigned char shaderModelMinor);
					
			public: void SkipOptimization(bool value);

			public: void AddDebugInfo(bool value);

			public: void AllowHighOptimizationTimes(bool value);

								
			public: DxPtr<ID3DBlob> MakeVertexShader(const wchar_t *filepath, const char *entrypoint);

				private: void DoCompile(const wchar_t *filepath, const char *entrypoint, const char *shaderTypePrefix, ID3DBlob **outBytecode, ID3DBlob **outError);
		
					private: UINT MakeCompileFlags();

			public: DxPtr<ID3DBlob> MakePixelShader(const wchar_t *filepath, const char *entrypoint);
			
		};

		
	}

	
}
