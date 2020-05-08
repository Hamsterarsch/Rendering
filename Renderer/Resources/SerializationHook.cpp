#include "Interface/Resources/SerializationHook.hpp"


namespace Renderer
{
	SerializationHook::Block::Block(SerializationHook *parent) : parent{parent} {}


	SerializationHook::Block::Block(Block &&other) noexcept : parent{ other.parent }
	{
		other.parent = nullptr;
		
	}


	
	SerializationHook::Block::~Block()
	{
		if(parent != nullptr)
		{
			parent->EndBlock();			
		}
		
	}



	
	

	SerializationHook::Block SerializationHook::BeginBlock(const size_t sizeInBytes)
	{
		lastBlockSizeInBytes = sizeInBytes;
		OnBeginBlock();
		
		return Block{ this };
		
	}

	
}
