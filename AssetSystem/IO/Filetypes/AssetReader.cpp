#include "AssetSystem/IO/Filetypes/AssetReader.hpp"
#include "AssetSystem/IO/Filetypes/AssetArchiveConstants.hpp"
#include "IO/DiskConversions.hpp"
#include "Shared/Exception/Exception.hpp"


namespace assetSystem::io
{//todo: catch eofs with exception
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
					   
			if(file.eof())
			{
				file.clear();
			}
					
		}

			void AssetReader::ProcessNextProperty()
			{
				if(!SeekNextPropertyStart())
				{
					return;
					
				}		
				auto propertyNameStart{ file.tellg() };
				
				if(SeekEofUntilToken(AssetArchiveConstants::dataStartToken))
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

						if(token == AssetArchiveConstants::dataStartToken)
						{
							return true;
							
						}						
					}
		
				}

					bool AssetReader::HandleObjectPropertyEnd()
					{
						PopCurrentObjectScope();

						if(SeekEofUntilToken(AssetArchiveConstants::dataStartToken))
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
						return;
						
					}

					if(propertyToken == AssetArchiveConstants::dataStartToken)
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
								|| character == AssetArchiveConstants::dataStartToken
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
									|| character == AssetArchiveConstants::dataStartToken
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
	
						SeekToTokenAfterPropertyValue();
		
					}

						void AssetReader::SeekToTokenAfterPropertyValue()
						{
							if(SkipValueSection() == std::ifstream::traits_type::eof())
							{
								return;
							}
								
							while(true)
							{
								const auto character{ GetNonBinaryDataFromFile() };
								if
								(
									character == std::ifstream::traits_type::eof()
									|| character == ','
									|| character == '}'
								)
								{
									if(character == '}')
									{
										//this character needs to be preserved for seek next property
										file.seekg(-1, std::ifstream::cur);
									}									
									return;
									
								}
							}
		
						}

							char AssetReader::SkipValueSection()
							{
								while(true)
								{
									const auto character{ GetNonBinaryDataFromFile() };
									if(character == std::ifstream::traits_type::eof())
									{
										return character;										
									}

									if(character == AssetArchiveConstants::dataStartToken)
									{
										return character;
									}									
								}
		
							}

							char AssetReader::GetNonBinaryDataFromFile()
							{
								if(FileIsAtBinaryDataStartSequence())
								{
									SkipBinaryDataToken();
									return GetFirstCharacterAfterBinary();									
								}
								return file.get();
		
							}

								void AssetReader::SkipBinaryDataToken()
								{
									file.seekg(AssetArchiveConstants::binaryTokenLength-1, std::ifstream::cur);
								
								}

								bool AssetReader::FileIsAtBinaryDataStartSequence()
								{
									const auto readToken{ file.peek() };
									if(readToken != AssetArchiveConstants::binaryStartToken[0])
									{
										return false;
										
									}

									const auto preCheckPos{ file.tellg() };
									std::string read(AssetArchiveConstants::binaryTokenLength-1, '\0');
									file.read(read.data(), AssetArchiveConstants::binaryTokenLength-1);
									file.seekg(preCheckPos);

									if(read != AssetArchiveConstants::binaryStartToken)
									{
										return false;
										
									}

									return true;
		
								}

								char AssetReader::GetFirstCharacterAfterBinary()
								{
									while(true)
									{
										const auto character{ file.peek() };
										if(character != AssetArchiveConstants::binaryEndToken[0])
										{
											file.get();
											continue;											
										}

										std::string read(AssetArchiveConstants::binaryTokenLength-1, '\0');
										file.read(read.data(), AssetArchiveConstants::binaryTokenLength-1);

										if(read == AssetArchiveConstants::binaryEndToken)
										{
											return file.get();
											
										}										
									}
		
								}

						void AssetReader::PopCurrentObjectScope()
						{
							objectQualifiers.pop_back();
							while(!objectQualifiers.empty())
							{								
								if(objectQualifiers.back() == '.')
								{
									break;
								}
								objectQualifiers.pop_back();
							}							
		
						}



	Archive &AssetReader::Serialize(const char *propertyName, unsigned char *data, const size_t numElements, const size_t elementStrideInBytes)
	{
		SeekPropertyValueStart(propertyName);
		
		SkipBinaryDataToken();
		if(IsBigEndianMachine() || elementStrideInBytes == 1)
		{
			file.read(reinterpret_cast<char *>(data), numElements*elementStrideInBytes);									
		}
		else
		{
			ReadFileForLittleEndian(data, numElements, elementStrideInBytes);			
		}
		SkipBinaryDataToken();
		
		return *this;
		
	}

		void AssetReader::ReadFileForLittleEndian(unsigned char *data, const size_t numElements, const size_t elementStrideInBytes)
		{
			for(size_t readElements{ 0 }; readElements < numElements; ++readElements)
			{
				auto *baseElement{ reinterpret_cast<char *>(data) + readElements * elementStrideInBytes };
				for(size_t byteToRead{ elementStrideInBytes }; byteToRead > 0; --byteToRead)
				{
					auto *targetByte{ baseElement + byteToRead -1 };
					file.read(targetByte, 1);
				}
			}
		
		}

		void AssetReader::SeekPropertyValueStart(const char *propertyName)
		{
			const auto afterNamePos{ propertyMap.at(objectQualifiers + propertyName) };
		
			file.seekg(afterNamePos);		

			SeekEofUntilToken(AssetArchiveConstants::dataStartToken);
			file.get();
		
		}



	Archive &AssetReader::Serialize(const char *propertyName, int32_t &data)
	{
		data = std::stoi(ReadPropertyValue(propertyName));
		
		return *this;
		
	}

		std::string AssetReader::ReadPropertyValue(const char *propertyName)
		{
			const auto valueLength{ SeekNonBinaryPropertyValueStartAndLength(propertyName) };

			std::string value(valueLength, '\0');			
			file.read(value.data(), valueLength);

			return value;
		
		}

			size_t AssetReader::SeekNonBinaryPropertyValueStartAndLength(const char *propertyName)
			{
				SeekPropertyValueStart(propertyName);
						
				const auto valueStart{ file.tellg() };

				SeekEofUntilToken(AssetArchiveConstants::dataStartToken);
				const auto valueEnd{ file.tellg() };
		
				file.seekg(valueStart);
				return valueEnd - valueStart;
		
			}


	
	Archive &AssetReader::Serialize(const char *propertyName, uint32_t &data)
	{
		data = static_cast<unsigned>(std::stoul(ReadPropertyValue(propertyName)));

		return *this;
		
	}



	Archive &AssetReader::Serialize(const char *propertyName, float &data)
	{
		data = std::stof(ReadPropertyValue(propertyName));
		
		return *this;
		
	}



	Archive &AssetReader::Serialize(const char *propertyName, char *str)
	{
		const auto valueLength{ SeekNonBinaryPropertyValueStartAndLength(propertyName) };				
		file.read(str, valueLength);

		return *this;
		
	}


	
	size_t AssetReader::GetPropertySizeInBytes(const char *propertyName)
	{		
		if(FileIsAtBinaryDataStartSequence())
		{
			SkipBinaryDataToken();
			const auto valueStart{ file.tellg() };

			GetFirstCharacterAfterBinary();
			const auto valueEnd{ file.tellg() };
			
			return valueEnd - valueStart;
			
		}		
		return SeekNonBinaryPropertyValueStartAndLength(propertyName);
		
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
