#pragma once	
#include "IO/Reader.hpp"
#include <fstream>
#include <filesystem>
#include <unordered_map>


namespace assetSystem::io
{
	class AssetReader final : public Reader
	{
		private: std::ifstream file;

		private: std::unordered_map<std::string, std::ifstream::pos_type> propertyMap;

		private: std::string objectQualifiers;

		private: std::ifstream::pos_type currentPropertyNameEnd;


				
		public: AssetReader(const std::filesystem::path &filepath);

			private: bool SeekEofUntilToken(char token);
		
			private: void BuildPropertyMap();

				private: void ProcessNextProperty();

					private: bool SeekNextPropertyStart();
							 							 
						private: bool HandleObjectPropertyEnd();

					private: void ProcessProperty(std::string &&propertyName);

						private: char GetPropertyToken();

						private: void ProcessObjectProperty(std::string &&propertyName);

							private: char GetFirstTokenInObject();
		
							private: void AddObjectScope(std::string &&objectName);

						private: void ProcessValueProperty(std::string &&propertyName);

							private: void SeekToTokenAfterPropertyValue();
		
								private: char SkipValueSection();

								private: char GetNonBinaryDataFromFile();

									private: bool FileIsAtBinaryDataStartSequence();

									private: char GetFirstCharacterAfterBinary();

										private: void SkipBinaryDataToken();
		
							private: void PopCurrentObjectScope();
		

		public: using ArchiveBase::Serialize;
	
		public: Archive &Serialize(const char *propertyName, unsigned char *data, size_t numElements, size_t elementStrideInBytes) override;
		
			private: void ReadFileForLittleEndian(unsigned char *data, size_t numElements, size_t elementStrideInBytes);
		
			private: void SeekPropertyValueStart(const char *propertyName);
		
		public: Archive &Serialize(const char *propertyName, int32_t &data) override;

			private: std::string ReadPropertyValue(const char *propertyName);

				private: size_t SeekNonBinaryPropertyValueStartAndLength(const char *propertyName);

		public: Archive &Serialize(const char *propertyName, uint32_t &data) override;
						 		
		public: Archive &Serialize(const char *propertyName, float &data) override;
		
		public: Archive &Serialize(const char *propertyName, char *str) override;

		public: size_t GetPropertySizeInBytes(const char *propertyName) override;
		
		
		public: bool IsInvalid() const override;
		
		public: void Close() override;
		
		public: Archive &EnterSubobject(const char *propertyName) override;
		
		public: Archive &LeaveSubobject() override;
						
	};

	
}
