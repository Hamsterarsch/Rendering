#include "AssetSystem/IO/Filetypes/AssetReader.hpp"


namespace AssetSystem::IO
{
	AssetReader::AssetReader(const std::filesystem::path &filepath) :
		file{ filepath, std::ios_base::in | std::ios_base::binary }
	{
		if(this->IsInvalid())
		{
			return;
			
		}

		BuildPropertyMap();
		
		//find beginning
		if(SeekEofUntilToken('{'))
		{
			return;
			
		}

		while(true)
		{
			//find property
			if(SeekEofUntilToken('"'))
			{
				return;
				
			}
			file.get();
			auto propertyNameStart{ file.tellg() };
			
			if(SeekEofUntilToken('"'))
			{
				return;
				
			}
			auto propertyNameEnd{ file.tellg() };

			file.seekg(propertyNameStart);
			const auto nameLength{  propertyNameEnd - propertyNameStart };

			std::string name(nameLength, '\0');			
			file.read(name.data(), nameLength);
			file.get();		

			while(file.peek() != std::ifstream::traits_type::eof())
			{
				if(file.peek() == '{')
				{
					if(!objectQualifiers.empty())
					{
						objectQualifiers += '.';
					}
					objectQualifiers += name + '.';

					while(file.peek() != std::ifstream::traits_type::eof())
					{
						const auto c{ file.peek() };
						if(c == '}')
						{
							while(!objectQualifiers.size() >= 2)
							{
								objectQualifiers.pop_back();
								if(objectQualifiers.back() == '.')
								{
									break;
								}
							}
							objectQualifiers.pop_back();
							break;
						}

						if(c == '"')
						{										
							break;
						}
						file.get();
						
					}
					break;
				}

				if(file.get() == '"')
				{
					propertyMap.insert( { objectQualifiers + std::move(name), 1+propertyNameEnd} );
					while(file.peek() != std::ifstream::traits_type::eof())
					{
						const auto c{ file.peek() };
						if(c == ',')
						{
							break;
						}

						if(c == '}')
						{
							if(objectQualifiers.empty())
							{
								return;
							}
							
							while(objectQualifiers.size() >= 2)
							{
								objectQualifiers.pop_back();
								if(objectQualifiers.back() == '.')
								{
									break;
								}
							}
							objectQualifiers.pop_back();
							break;
						}

						if(c == std::ifstream::traits_type::eof())
						{
							return;
						}
						file.get();
						
					}
					
					break;
				}
				
			}


						
		}
		
		
	}

		void AssetReader::BuildPropertyMap()
		{

		
		}

		bool AssetReader::SeekEofUntilToken(const char token)
		{			
			while(file.peek() != std::ifstream::traits_type::eof())
			{				
				if(file.peek() == token)
				{
					return false;
					
				}
				file.get();
			}
			return true;
		
		}



	Archive &AssetReader::Serialize(const char *propertyName, unsigned char *data, size_t sizeInBytes)
	{
		SeekPropertyValueStart(propertyName);

		file.read(reinterpret_cast<char *>(data), sizeInBytes);		
		
		return *this;
	}

		void AssetReader::SeekPropertyValueStart(const char *propertyName)
		{
			const auto afterNamePos{ propertyMap.at(objectQualifiers + propertyName) };
			file.seekg(afterNamePos);		
		
			SeekEofUntilToken('"');
			file.get();
		
		}



	Archive &AssetReader::Serialize(const char *propertyName, int &data)
	{
		data = std::stoi(ReadPropertyValue(propertyName));
		
		return *this;
		
	}

		std::string AssetReader::ReadPropertyValue(const char *propertyName)
		{
			const auto valueLength{ SeekPropertyValueStartAndLength(propertyName) };

			std::string value(valueLength, '\0');			
			file.read(value.data(), valueLength);

			return value;
		
		}

			size_t AssetReader::SeekPropertyValueStartAndLength(const char *propertyName)
			{
				SeekPropertyValueStart(propertyName);		
				const auto valueStart{ file.tellg() };

				SeekEofUntilToken('"');
				const auto valueEnd{ file.tellg() };
		
				file.seekg(valueStart);
				return valueEnd - valueStart;
		
			}



	Archive &AssetReader::Serialize(const char *propertyName, float &data)
	{
		data = std::stof(ReadPropertyValue(propertyName));
		
		return *this;
		
	}



	Archive &AssetReader::Serialize(const char *propertyName, char *str)
	{
		const auto valueLength{ SeekPropertyValueStartAndLength(propertyName) };				
		file.read(str, valueLength);
		
		return *this;
		
	}



	bool AssetReader::IsInvalid() const
	{
		return !file.is_open();
		
	}



	void AssetReader::Close()
	{
		file.close();
		
	}



	Archive &AssetReader::EnterSubobject(const char *propertyName)
	{
		if(!objectQualifiers.empty())
		{
			objectQualifiers += '.';
		}
		objectQualifiers += propertyName;
		objectQualifiers += '.';
		
		return *this;
	}



	Archive &AssetReader::LeaveSubobject()
	{
		while(objectQualifiers.size() >= 2)
		{
			objectQualifiers.pop_back();
			if(objectQualifiers.back() == '.')
			{
				break;
			}
		}
		objectQualifiers.pop_back();
		
		return *this;
	}

	
}
