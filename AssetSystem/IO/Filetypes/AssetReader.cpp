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
				
		//find beginning
		if(SeekEofUntilToken('{'))
		{
			return;
			
		}

		BuildPropertyMap();
		return;
				
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

		void AssetReader::BuildPropertyMap()
		{
			ProcessNextProperty();
		
		}

			void AssetReader::ProcessNextProperty()
			{
				if(!SeekNextPropertyStart())
				{
					return;
					
				}		
				auto propertyNameStart{ file.tellg() };
				
				if(SeekEofUntilToken('"'))
				{
					return;
					
				}
				currentPropertyNameEnd = file.tellg();

				file.seekg(propertyNameStart);
				const auto nameLength{  currentPropertyNameEnd - propertyNameStart };

				std::string name(nameLength, '\0');			
				file.read(name.data(), nameLength);
				file.get();

				ProcessProperty(std::move(name));
				ProcessNextProperty();
		
			}

				bool AssetReader::SeekNextPropertyStart()
				{					
					while(true)
					{
						if(file.peek() == std::ifstream::traits_type::eof())
						{
							return false;
							
						}
						
						const auto token{ file.get() };
						if(token == '}')
						{
							if(objectQualifiers.empty())
							{
								return false;
								
							}
							return HandleObjectPropertyEnd();							
						}

						if(token == '"')
						{
							return true;
							
						}						
					}
		
				}

					bool AssetReader::HandleObjectPropertyEnd()
					{
						PopCurrentObjectScope();

						if(SeekEofUntilToken('"'))
						{
							return false;
						}
						file.get();
						return true;
		
					}

				void AssetReader::ProcessProperty(std::string &&propertyName)
				{
					const auto propertyToken{ GetPropertyToken() };
					if(propertyToken == std::ifstream::traits_type::eof())
					{
						return;
						
					}

					if(propertyToken == '{')
					{
						ProcessObjectProperty(std::move(propertyName));
					}

					if(propertyToken == '"')
					{
						ProcessValueProperty(std::move(propertyName));
					}
		
				}

					char AssetReader::GetPropertyToken()					
					{
						while(true)
						{
							const auto character{ file.get() };
							if
							(
								character == std::ifstream::traits_type::eof()
								|| character == '{'
								|| character == '"'
							)
							{
								return character;
								
							}														
						}
		
					}

					void AssetReader::ProcessObjectProperty(std::string &&propertyName)
					{
						const auto firstToken{ GetFirstTokenInObject() };
						if
						(	
							firstToken == std::ifstream::traits_type::eof()
							|| firstToken == '}'
						)
						{
							return;
							
						}
		
						AddObjectScope(std::move(propertyName));
						file.seekg(-1, std::ifstream::cur);
					
					}

						char AssetReader::GetFirstTokenInObject()
						{
							while(true)
							{
								const auto character{ file.get() };
								if
								(
									character == std::ifstream::traits_type::eof()
									|| character == '}'
									|| character == '"'
								)
								{
									return character;
									
								}
							}
		
						}

						void AssetReader::AddObjectScope(std::string &&objectName)
						{
							objectQualifiers += std::move(objectName) + '.';
		
						}

					void AssetReader::ProcessValueProperty(std::string&& propertyName)
					{
						propertyMap.insert( { objectQualifiers + std::move(propertyName), 1+currentPropertyNameEnd} );
						const auto token{ GetNextTokenAfterValueProperty() };
		
						if(token == ',')
						{
							return;
							
						}

						if(token == '}')
						{
							PopCurrentObjectScope();							
						}
								
					}

						char AssetReader::GetNextTokenAfterValueProperty()
						{
							while(true)
							{
								const auto character{ file.get() };
								if
								(
									character == std::ifstream::traits_type::eof()
									|| character == ','
									|| character == '}'
								)
								{
									return character;
									
								}
							}
		
						}

						void AssetReader::PopCurrentObjectScope()
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
		AddObjectScope({propertyName});
		
		return *this;
		
	}



	Archive &AssetReader::LeaveSubobject()
	{
		PopCurrentObjectScope();
		
		return *this;
		
	}

	
}
