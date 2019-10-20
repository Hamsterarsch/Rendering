#pragma once
#include "DX12/ShaderFactory.hpp"
#include <string>



namespace RHA
{
	namespace DX12
	{
		class ShaderFactoryImpl : public ShaderFactory
		{
			std::string shaderModelSpec;
			
			bool insertDebugInfo;
			
			bool skipOptimization;
			
			bool useHighOptimization;


			
			public: ShaderFactoryImpl(unsigned char shaderModelMajor, unsigned char shaderModelMinor);
			
			public: virtual void SkipOptimization(bool value) override;

			public: virtual void AddDebugInfo(bool value) override;

			public: virtual void AllowHighOptimizationTimes(bool value) override;

								
			public: virtual DxPtr<ID3DBlob> MakeVertexShader(const wchar_t *filepath, const char *entrypoint) override;

				private: void DoCompile(const wchar_t *filepath, const char *entrypoint, const char *shaderTypePrefix, ID3DBlob **outBytecode, ID3DBlob **outError);
		
					private: UINT MakeCompileFlags();

			public: virtual DxPtr<ID3DBlob> MakePixelShader(const wchar_t *filepath, const char *entrypoint) override;
			
		};

		
	}

	
}
