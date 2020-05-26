#pragma once
#include <cstdint>


namespace AssetSystem::IO
{
	inline bool IsBigEndianMachine()
	{
		const uint32_t i{ 0x01'00'00'00 };
		const auto *bytes{ reinterpret_cast<const unsigned char *>(&i) };

		return bytes[0] > 0;
		
	}
	
}
