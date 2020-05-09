#pragma once
#include <d3d12.h>
#include <vector>
#include <unordered_map>

namespace RHA
{
	namespace DX12
	{
		class IncludeHandler final : public ID3DInclude
		{
			private: std::vector<std::string> includeDirectories;

			
			
			public: HRESULT __stdcall Open
			(
				D3D_INCLUDE_TYPE IncludeType,
				LPCSTR pFileName,
				LPCVOID pParentData,
				LPCVOID *ppData,
				UINT *pBytes
			) override;
			
			public: HRESULT __stdcall Close(LPCVOID pData) override;
					
			public: void AddIncludeDirectory(const char *directory);
											
		};
		
		
	}

	
}
