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

								
			public: virtual DxPtr<ID3DBlob> MakeVertexShader(const wchar_t *filepath, const char *entrypoint) const override;

				private: DxPtr<ID3DBlob> DoCompileFromFile(const wchar_t *filepath, const char *entrypoint, const char *shaderTypePrefix) const;
		
					private: UINT MakeCompileFlags() const;

				private: static void CheckCompilationFromFile(HRESULT result, const DxPtr<ID3DBlob> &errorBlob);

			public: virtual DxPtr<ID3DBlob> MakeVertexShader(const char *shader, size_t shaderLength, const char *entrypoint) const override;

				private: DxPtr<ID3DBlob> DoCompile(const char *shader, size_t shaderLength, const char *entrypoint, const char *shaderTypePrefix) const;

					private: static void CheckCompilation(HRESULT result, const DxPtr<ID3DBlob> &errorBlob);
			
			
			public: virtual DxPtr<ID3DBlob> MakePixelShader(const wchar_t *filepath, const char *entrypoint) const override;

			public: virtual DxPtr<ID3DBlob> MakePixelShader(const char *shader, size_t shaderLength, const char *entrypoint) const override;
			
		};

		
	}

	
}
