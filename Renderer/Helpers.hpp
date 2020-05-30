#pragma once
#include <d3d12.h>


namespace Renderer::DX12
{
	inline void NameDX12ObjectIfDebug(ID3D12Object *object, const wchar_t *name)
	{
		#ifdef _DEBUG
			object->SetName(name);
		#endif
		
	}
	
}
