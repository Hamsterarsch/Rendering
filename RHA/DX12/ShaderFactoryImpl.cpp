#include "DX12/ShaderFactoryImpl.hpp"
#include <d3dcompiler.h>
#include "Shared/Exception/CreationFailedException.hpp"
#include "IncludeHandler.hpp"


namespace RHA
{
	namespace DX12
	{
		ShaderFactoryImpl::ShaderFactoryImpl(const unsigned char shaderModelMajor, const unsigned char shaderModelMinor) :
			insertDebugInfo{ false },
			skipOptimization{ false },
			useHighOptimization{ false }
		{
			shaderModelSpec = "_" + std::to_string(shaderModelMajor) + "_" + std::to_string(shaderModelMinor);

		}


		
		void ShaderFactoryImpl::SkipOptimization(const bool value)
		{
			skipOptimization = value;
			
		}


		
		void ShaderFactoryImpl::AddDebugInfo(const bool value)
		{
			insertDebugInfo = value;
			
		}


		
		void ShaderFactoryImpl::AllowHighOptimizationTimes(const bool value)
		{
			useHighOptimization = value;
			
		}


		
		void ShaderFactoryImpl::AddIncludeDirectory(const char *directory)
		{
			includeHandler.AddIncludeDirectory(directory);
			
		}

		

		DxPtr<ID3DBlob> ShaderFactoryImpl::MakeVertexShader(const wchar_t *filepath, const char *entrypoint) 
		{			
			return DoCompileFromFile(filepath, entrypoint, "vs");
			
		}

			DxPtr<ID3DBlob> ShaderFactoryImpl::DoCompileFromFile(const wchar_t *filepath, const char *entrypoint, const char *shaderTypePrefix) 
			{
				static constexpr auto NO_DEFINES{ nullptr };
				DxPtr<ID3DBlob> shaderBlob{ nullptr }, errorBlob{ nullptr };
								
				const auto result
				{
					D3DCompileFromFile
					(
						filepath,
						NO_DEFINES,
						&includeHandler,
						entrypoint,
						(shaderTypePrefix + shaderModelSpec).data(),
						MakeCompileFlags(),
						0,
						&shaderBlob,
						&errorBlob
					)
				};				
				CheckCompilationFromFile(result, errorBlob);
				
				return shaderBlob;
			
			}

				UINT ShaderFactoryImpl::MakeCompileFlags() const
				{
					UINT Flags{ 0 };
					Flags |= insertDebugInfo ? D3DCOMPILE_DEBUG : 0;
					Flags |= skipOptimization ? D3DCOMPILE_SKIP_OPTIMIZATION : 0;
					Flags |= useHighOptimization ? D3DCOMPILE_OPTIMIZATION_LEVEL3 : 0;

					return Flags;

				}

				void ShaderFactoryImpl::CheckCompilationFromFile(const HRESULT result, const DxPtr<ID3DBlob> &errorBlob)
				{
					if (FAILED(result))
					{
						std::string error{ "Could not compile hlsl shader from file" };
						if(errorBlob)
						{
							error += ". \nError: ";
							error += reinterpret_cast<char *>(errorBlob->GetBufferPointer());						
						}
						
						throw Exception::CreationFailed{ error.data() };
					}
			
				}


		
		DxPtr<ID3DBlob> ShaderFactoryImpl::MakeVertexShader(const char *shader, size_t shaderLength, const char *entrypoint)
		{
			return DoCompile(shader, shaderLength, entrypoint, "vs");
			
		}

			DxPtr<ID3DBlob> ShaderFactoryImpl::DoCompile
			(
				const char *shader,
				size_t shaderLength, 
				const char *entrypoint,
				const char *shaderTypePrefix
			)			
			{
				static constexpr auto UNNAMED{ nullptr }, NO_DEFINES{ nullptr };
				DxPtr<ID3DBlob> shaderBlob{ nullptr }, errorBlob{ nullptr };
							
				const auto result
				{
					D3DCompile
					(
						shader,
						shaderLength,
						UNNAMED,
						NO_DEFINES,
						&includeHandler,
						entrypoint,
						(shaderTypePrefix + shaderModelSpec).data(),
						MakeCompileFlags(),
						0,
						&shaderBlob,
						&errorBlob
					)
				};
				CheckCompilation(result, errorBlob);

				return shaderBlob;
				
			}

				void ShaderFactoryImpl::CheckCompilation(const HRESULT result, const DxPtr<ID3DBlob> &errorBlob)
				{
					if (FAILED(result))
					{
						std::string error{ "Could not compile hlsl shader" };
						if(errorBlob)
						{
							error += ". \nError: ";
							error += reinterpret_cast<char *>(errorBlob->GetBufferPointer());						
						}
						
						throw Exception::CreationFailed{ error.data() };
					}
			
				}


		
		DxPtr<ID3DBlob> ShaderFactoryImpl::MakePixelShader(const wchar_t *filepath, const char *entrypoint) 
		{			
			return DoCompileFromFile(filepath, entrypoint, "ps");
						
		}


		
		DxPtr<ID3DBlob> ShaderFactoryImpl::MakePixelShader
		(
			const char *shader, 
			const size_t shaderLength,
			const char *entrypoint
		)		
		{
			return DoCompile(shader, shaderLength, entrypoint, "ps");
			
		}


		
		DxPtr<ID3DBlob> ShaderFactoryImpl::MakeComputeShader
		(
			const char *shader,
			const size_t shaderLength,
			const char *entrypoint
		) 
		{
			return DoCompile(shader, shaderLength, entrypoint, "cs");
			
		}

		
	}

	
}
