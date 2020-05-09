#include "DX12/IncludeHandler.hpp"
#include <fstream>
#include "Shared/PtrTypes.hpp"

namespace RHA
{
	namespace DX12
	{
		HRESULT IncludeHandler::Open
		(
			const D3D_INCLUDE_TYPE IncludeType,
			const LPCSTR pFileName,
			const LPCVOID pParentData,
			LPCVOID *ppData,
			UINT *pBytes
		)
		{
			try
			{
				for(auto &&directory : includeDirectories)
				{
					std::ifstream includefile{directory + '/' + pFileName, std::ios_base::in | std::ios_base::ate | std::ios_base::binary};

					if(!includefile.is_open())
					{
						continue;
					}

					const auto charCount{ includefile.tellg() };
					*pBytes = charCount * sizeof(char);					
					includefile.seekg(0);

					
					auto include{ std::make_unique<char[]>(charCount) };
					includefile.read(include.get(), charCount);
					*ppData = include.release();
															
					return S_OK;
					
				}
			}
			catch(...)
			{
				return E_FAIL;
				
			}

			return E_FAIL;
							
		}
		

		HRESULT IncludeHandler::Close(LPCVOID pData)
		{
			delete[] static_cast<const char *>(pData);
			
			return S_OK;
				
		}
		

		void IncludeHandler::AddIncludeDirectory(const char *directory)
		{
			includeDirectories.emplace_back(directory);
				
		}
		
		
	}

	
}