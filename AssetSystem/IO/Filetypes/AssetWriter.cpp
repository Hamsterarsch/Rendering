#include "AssetSystem/IO/Filetypes/AssetWriter.hpp"
#include <fstream>
#include "AssetSystem/IO/Filetypes/AssetArchiveConstants.hpp"
#include "IO/DiskConversions.hpp"


namespace AssetSystem::IO
{
	AssetWriter::AssetWriter(const std::filesystem::path &filepath) :
		file{ filepath, std::ios_base::out | std::ios_base::trunc },
		hasWrittenPropertyValue{ false }
	{				
		if(IsInvalid())
		{
			return;
			
		}

		file << "{\n";		
					
	}


	
	AssetWriter::~AssetWriter() noexcept
	{
		try
		{
			Close();
			
		} catch(...) {}
		
	}



	Archive &AssetWriter::Serialize(const char *propertyName, unsigned char* data, const size_t numElements, const size_t elementStrideInBytes)
	{		
		WritePropertyName(propertyName);
		WritePropertyValue(data, numElements * elementStrideInBytes, elementStrideInBytes, true);

		return *this;
		
	}

		void AssetWriter::WritePropertyName(const char *propertyName)
		{
			WritePropertyDelimiters();
			file << '\t' << subobjectTabs << '"' << propertyName << "\": ";
		
		}

			void AssetWriter::WritePropertyDelimiters()
			{
				if(hasWrittenPropertyValue)
				{
					file << propertyDelimiters;
				}
		
			}
				
		void AssetWriter::WritePropertyValue(const unsigned char* data, const size_t sizeInBytes, const size_t elementStrideInBytes, const bool isBinary)
		{
			file << '"';
		
			if(isBinary)
			{
				file << AssetArchiveConstants::binaryStartToken;
				if(IsBigEndianMachine() || elementStrideInBytes == 1)
				{
					file.write(reinterpret_cast<const char *>(data), sizeInBytes);						
				}
				else
				{
					for(size_t writtenElements{ 0 }; writtenElements < sizeInBytes / elementStrideInBytes; ++writtenElements)
					{
						const auto *baseElement{ data + writtenElements * elementStrideInBytes };
						for(size_t byteToWrite{ elementStrideInBytes }; byteToWrite > 0; --byteToWrite)
						{
							const auto *targetByte{ baseElement + byteToWrite-1 };
							file.put(*targetByte);
						}
					}
					
				}							
				file << AssetArchiveConstants::binaryEndToken;
			}
			else
			{
				file.write(reinterpret_cast<const char *>(data), sizeInBytes);				
			}
		
			file << '"';
			hasWrittenPropertyValue = true;
		
		}
	
	Archive &AssetWriter::Serialize(const char *propertyName, int32_t &data) 
	{
		WritePropertyName(propertyName);
						
		const auto asString{ std::to_string(data) };
		WritePropertyValue(reinterpret_cast<const unsigned char *>(asString.c_str()), asString.size(), sizeof(std::string::value_type));

		return *this;
		
	}


	
	Archive &AssetWriter::Serialize(const char *propertyName, float &data) 
	{
		WritePropertyName(propertyName);
				
		const auto asString{ std::to_string(data) };
		WritePropertyValue(reinterpret_cast<const unsigned char *>(asString.c_str()), asString.size(), sizeof(std::string::value_type));

		return *this;
		
	}


	
	Archive &AssetWriter::Serialize(const char *propertyName, char *str) 
	{
		WritePropertyName(propertyName);
				
		file << '"';
		file << str;
		file << '"';
		hasWrittenPropertyValue = true;

		return *this;
		
	}



	bool AssetWriter::IsInvalid() const
	{
		return !file.is_open();
		
	}


	
	void AssetWriter::Close()
	{
		while(!subobjectTabs.empty())
		{
			subobjectTabs.pop_back();
			file << "\n\t" << subobjectTabs << "}";
		}
		
		file << "\n}";
		file.close();
		
	}


	
	Archive &AssetWriter::EnterSubobject(const char *propertyName)
	{
		WritePropertyName(propertyName);

		file << "\n\t" << subobjectTabs << "{\n";
		subobjectTabs.push_back('\t');
		
		hasWrittenPropertyValue = false;

		return *this;
		
	}


	
	Archive &AssetWriter::LeaveSubobject()
	{
		subobjectTabs.pop_back();

		if(hasWrittenPropertyValue)
		{
			file << "\n";
		}

		file << '\t' <<  subobjectTabs << "}";

		hasWrittenPropertyValue = true;

		return *this;
		
	}

	
}
