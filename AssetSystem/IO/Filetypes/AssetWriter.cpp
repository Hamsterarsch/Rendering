#include "AssetSystem/IO/Filetypes/AssetWriter.hpp"
#include <fstream>
#include "AssetSystem/IO/Filetypes/AssetArchiveConstants.hpp"


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



	Archive &AssetWriter::Serialize(const char *propertyName, unsigned char* data, const size_t sizeInBytes)
	{		
		WritePropertyName(propertyName);
		WritePropertyValue(data, sizeInBytes, true);

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
				
		void AssetWriter::WritePropertyValue(const unsigned char* data, const size_t sizeInBytes, const bool isBinary)
		{
			file << '"';
			if(isBinary)
			{
				file << AssetArchiveConstants::binaryStartToken;
			}
		
			file.write(reinterpret_cast<const char *>(data), sizeInBytes);

			if(isBinary)
			{
				file << AssetArchiveConstants::binaryEndToken;
			}
			file << '"';
			hasWrittenPropertyValue = true;
		
		}
	
	Archive &AssetWriter::Serialize(const char *propertyName, int &data) 
	{
		WritePropertyName(propertyName);
						
		const auto asString{ std::to_string(data) };
		WritePropertyValue(reinterpret_cast<const unsigned char *>(asString.c_str()), asString.size());

		return *this;
		
	}


	
	Archive &AssetWriter::Serialize(const char *propertyName, float &data) 
	{
		WritePropertyName(propertyName);
				
		const auto asString{ std::to_string(data) };
		WritePropertyValue(reinterpret_cast<const unsigned char *>(asString.c_str()), asString.size());

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
