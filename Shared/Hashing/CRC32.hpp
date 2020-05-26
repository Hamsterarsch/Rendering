#pragma once
#include <cstdint>
#include <cstring>

inline bool IsLittleEndian()
{
	uint32_t i{ 1 };
	unsigned char c[4]{};
	std::memcpy(c, &i, sizeof(i));
	
	return c[0] == 1;
	
}


namespace Hashing
{	
	class  CRC32LookupTable
	{
		private: unsigned table[256];
	
		private: unsigned polynomial;

				 

		public: constexpr CRC32LookupTable(const unsigned polynomial) : table{}, polynomial{ polynomial }
		{
			for(unsigned index{0}; index < 256; ++index)
			{							
				table[index] = GenerateTransformation(index);				
			}
			
		}

			private: constexpr unsigned GenerateTransformation(const unsigned char forIndex) const
			{
				auto controlByte{ static_cast<unsigned char>(forIndex) };
							
				unsigned transform{ 0 };
				for(unsigned iteration{0}; iteration < sizeof(char)*8; ++iteration)
				{				
					const unsigned char controlBit{ GetControlBit(controlByte, iteration) };		
	
					controlByte ^= ShiftPolynomialToMatchControlValues(controlBit * polynomial, iteration);
					transform	^= ShiftPolynomialToMatchRegister(controlBit * polynomial, iteration);
				}
				return transform;
				
			}
		
				private: static constexpr unsigned char GetControlBit(const unsigned char controlByte, const unsigned char iteration)
				{		
					return static_cast<unsigned char>(controlByte << iteration) >> (sizeof(char)*8-1);
								
				}
		
				private: static constexpr unsigned ShiftPolynomialToMatchControlValues(const unsigned polynomial, const unsigned char iteration)
				{
					return polynomial >> ((sizeof(unsigned)-1)*8 + iteration+1);
					
				}
	
				private: static constexpr unsigned ShiftPolynomialToMatchRegister(const unsigned polynomial, const unsigned char iteration)
				{
					return polynomial << (sizeof(char)*8 - iteration-1);
					
				}
	

	
		public: constexpr unsigned operator[](const unsigned char index) const
		{
			return table[index];
			
		}
		
	};


	
	class CRC32Mpeg2
	{
		static constexpr unsigned polynomial{ 0x04C11DB7 };	
	
		static constexpr CRC32LookupTable table{ polynomial };

		
		//data has to be little endian
		public: static constexpr unsigned Generate(const char *data, size_t length)
		{			
			unsigned r{ 0xFFFF'FFFF};			
			while (length--)
			{				
				r = (r<<8) ^ table[(r >> 24) ^ *data++];			
				
			}
	
			return r;		
			
		}
		
	};

	
}