#include "DX12/ShaderFactoryImpl.hpp"
#include <d3dcompiler.h>
#include "Shared/Exception/CreationFailedException.hpp"


namespace RHA
{
	namespace DX12
	{
		ShaderFactoryImpl::ShaderFactoryImpl(unsigned char shaderModelMajor, unsigned char shaderModelMinor) :
			insertDebugInfo{ false },
			skipOptimization{ false },
			useHighOptimization{ false }
		{
			shaderModelSpec = "_" + std::to_string(shaderModelMajor) + "_" + std::to_string(shaderModelMinor);

		}

		void ShaderFactoryImpl::SkipOptimization(bool value)
		{
			skipOptimization = value;
			
		}

		void ShaderFactoryImpl::AddDebugInfo(bool value)
		{
			insertDebugInfo = value;
			
		}

		void ShaderFactoryImpl::AllowHighOptimizationTimes(bool value)
		{
			useHighOptimization = value;
			
		}

		DxPtr<ID3DBlob> ShaderFactoryImpl::MakeVertexShader(const wchar_t *filepath, const char *entrypoint)
		{
			DxPtr<ID3DBlob> bytecode;
			DxPtr<ID3DBlob> error;

			DoCompile(filepath, entrypoint, "vs", &bytecode, &error);

			return bytecode;

		}

			void ShaderFactoryImpl::DoCompile(const wchar_t *filepath, const char *entrypoint, const char *shaderTypePrefix, ID3DBlob **outBytecode, ID3DBlob **outError)
			{
				constexpr decltype(nullptr) NONE{ nullptr };

				const auto result
				{
					D3DCompileFromFile
					(
						filepath,
						NONE,
						NONE,
						entrypoint,
						(shaderTypePrefix + shaderModelSpec).data(),
						MakeCompileFlags(),
						0,
						outBytecode,
						outError
					)
				};
				
				if (FAILED(result))
				{
					std::string error{ "Could not compile hlsl shader from file" };
					if(outError)
					{
						error += ". \nError: ";
						error += reinterpret_cast<char *>((*outError)->GetBufferPointer());						
					}
					
					throw Exception::CreationFailed{ error.data() };
				}

			}

				UINT ShaderFactoryImpl::MakeCompileFlags()
				{
					UINT Flags{ 0 };
					Flags |= (insertDebugInfo ? D3DCOMPILE_DEBUG : 0);
					Flags |= (skipOptimization ? D3DCOMPILE_SKIP_OPTIMIZATION : 0);
					Flags |= (useHighOptimization ? D3DCOMPILE_OPTIMIZATION_LEVEL3 : 0);

					return Flags;

				}

		DxPtr<ID3DBlob> ShaderFactoryImpl::MakePixelShader(const wchar_t *filepath, const char *entrypoint)
		{
			DxPtr<ID3DBlob> bytecode;
			DxPtr<ID3DBlob> error;

			DoCompile(filepath, entrypoint, "ps", &bytecode, &error);

			return bytecode;
			
		}

		
	}

	
}