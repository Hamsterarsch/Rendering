#pragma once
#include "Shared/PtrTypes.hpp"
#include "SerializationHook.hpp"

namespace Renderer
{
	class RENDERER_DLLSPEC SerializeContainer : public SerializationHook
	{		
		private: UniquePtr<unsigned char[]> saved;

		private: size_t currentWriteOffset{ 0 };

		private: size_t size{ 0 };

		
		public:	void WriteToBlock(const void *data, size_t sizeInBytes) override;
		
		protected: void OnBeginBlock() override;
		
		public: const unsigned char *GetData() const { return saved.get(); }

		public: size_t GetSize() const { return size; }

		public: void Reset() { size = 0; currentWriteOffset = 0; saved.reset(); }
		
		
	};
	
	
}
