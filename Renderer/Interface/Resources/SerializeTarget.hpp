#pragma once
#include "Shared/PtrTypes.hpp"
#include "SerializationHook.hpp"

namespace Renderer
{
	class RENDERER_DLLSPEC SerializeTarget final : public SerializationHook
	{		
		private: UniquePtr<unsigned char[]> data;
				 
		private: size_t size{ 0 };

		
		
		public: const unsigned char *GetData() const override { return data.get(); }
		
		public: unsigned char *GetData() override { return data.get(); }

		public: size_t GetSizeInBytes() const { return size; }

		public: void Resize(size_t sizeInBytes) override;
							   		
	};
	
	
}
