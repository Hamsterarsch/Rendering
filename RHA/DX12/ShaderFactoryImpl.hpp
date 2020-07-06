#pragma once
#include "DX12/ShaderFactory.hpp"
#include <string>
#include "IncludeHandler.hpp"


namespace RHA::DX12
{
	class ShaderFactoryImpl final : public ShaderFactory
	{
		private: std::string shaderModelSpec;
		
		private: bool insertDebugInfo;
		
		private: bool skipOptimization;
		
		private: bool useHighOptimization;

		private: IncludeHandler includeHandler;

		private: std::string lastCompileMessage;

		private: bool compileFailed;

		private: bool compiledWithWarnings;


		
		public: ShaderFactoryImpl(unsigned char shaderModelMajor, unsigned char shaderModelMinor);
		
		public: void SkipOptimization(bool value) override;

		public: void AddDebugInfo(bool value) override;

		public: void AllowHighOptimizationTimes(bool value) override;

		public: void AddIncludeDirectory(const char *directory) override;

							
		public: DxPtr<ID3DBlob> MakeVertexShader(const wchar_t *filepath, const char *entrypoint) override;

			private: DxPtr<ID3DBlob> DoCompileFromFile(const wchar_t *filepath, const char *entrypoint, const char *shaderTypePrefix);
	
				private: UINT MakeCompileFlags() const;

				private: void CheckCompilation(HRESULT result, const DxPtr<ID3DBlob> &errorBlob);
		

		public: DxPtr<ID3DBlob> MakeVertexShader(const char *shader, size_t shaderLength, const char *entrypoint) override;

			private: DxPtr<ID3DBlob> DoCompile(const char *shader, size_t shaderLength, const char *entrypoint, const char *shaderTypePrefix);

		
		
		public: DxPtr<ID3DBlob> MakePixelShader(const wchar_t *filepath, const char *entrypoint) override;

		public: DxPtr<ID3DBlob> MakePixelShader(const char *shader, size_t shaderLength, const char *entrypoint) override;

		
		public: DxPtr<ID3DBlob> MakeComputeShader(const char* shader, size_t shaderLength, const char* entrypoint) override;

		
		public: bool WasCompileSuccessful() const override { return not compileFailed; }
		
		public: std::string GetCompileMessage() const override { return lastCompileMessage; }
		
		public: bool WasCompiledWithWarnings() const override{ return compiledWithWarnings; }
		
	};

		
}

	
