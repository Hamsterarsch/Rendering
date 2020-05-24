#include "AssetSystem/IO/Filetypes/AssetWriter.hpp"
#include <fstream>


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



	Archive &AssetWriter::SerializeBlob(const char *propertyName, unsigned char* data, const size_t sizeInBytes)
	{		
		WritePropertyName(propertyName);
		WritePropertyType("bin");
		WritePropertyValue(data, sizeInBytes);

		return *this;
		
	}

		void AssetWriter::WritePropertyName(const char *propertyName)
		{
			WritePropertyDelimiters();
			file << '\t' << subobjectTabs << '"' << propertyName << '"';
		
		}

			void AssetWriter::WritePropertyDelimiters()
			{
				if(hasWrittenPropertyValue)
				{
					file << propertyDelimiters;
				}
		
			}

		void AssetWriter::WritePropertyType(const char *token)
		{
			file << ":" << token << ": ";
		
		}

		void AssetWriter::WritePropertyValue(const unsigned char* data, size_t sizeInBytes)
		{
			file << '"';
			file.write(reinterpret_cast<const char *>(data), sizeInBytes);
			file << '"';
			hasWrittenPropertyValue = true;
		
		}



	Archive &AssetWriter::SerializeUint32(const char *propertyName, unsigned char *data)
	{
		WritePropertyName(propertyName);
		WritePropertyType("uin");

		uint32_t asUint;
		std::memcpy(&asUint, data, sizeof(uint32_t));
		
		const auto asString{ std::to_string(asUint) };
		WritePropertyValue(reinterpret_cast<const unsigned char *>(asString.c_str()), asString.size());

		return *this;
		
	}


	
	Archive &AssetWriter::SerializeInt32(const char *propertyName, unsigned char *data) 
	{
		WritePropertyName(propertyName);
		WritePropertyType("int");

		int32_t asInt;
		std::memcpy(&asInt, data, sizeof(int32_t));
		
		const auto asString{ std::to_string(asInt) };
		WritePropertyValue(reinterpret_cast<const unsigned char *>(asString.c_str()), asString.size());

		return *this;
		
	}


	
	Archive &AssetWriter::SerializeFloat(const char *propertyName, unsigned char *data) 
	{
		WritePropertyName(propertyName);
		WritePropertyType("flt");
		
		float asFloat;
		std::memcpy(&asFloat, data, sizeof(float));
		
		const auto asString{ std::to_string(asFloat) };
		WritePropertyValue(reinterpret_cast<const unsigned char *>(asString.c_str()), asString.size());

		return *this;
		
	}


	
	Archive &AssetWriter::SerializeString(const char *propertyName, char *str) 
	{
		WritePropertyName(propertyName);
		WritePropertyType("txt");
				
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
		WritePropertyType("obj");

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
