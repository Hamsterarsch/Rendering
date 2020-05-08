#include "Resources/SerializationContainer.hpp"

namespace Renderer
{
	void SerializeContainer::WriteToBlock(const void *data, const size_t sizeInBytes)
	{
		auto *bytePtr{ reinterpret_cast<const unsigned char *>(data) };
		memcpy(saved.get() + currentWriteOffset, data, sizeInBytes);

		currentWriteOffset += sizeInBytes;
			
	}

	void SerializeContainer::OnBeginBlock()
	{
		size = GetBlockSize();
		saved = std::make_unique<unsigned char[]>(size);

	}

	
}
