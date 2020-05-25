#pragma once	
#include "IO/Reader.hpp"
#include <fstream>
#include <filesystem>
#include <unordered_map>


namespace AssetSystem::IO
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

							private: char GetNextTokenAfterValueProperty();

							private: void PopCurrentObjectScope();
		
						
		public: Archive &Serialize(const char *propertyName, unsigned char *data, size_t sizeInBytes) override;
		
			private: void SeekPropertyValueStart(const char *propertyName);
		
		public: Archive &Serialize(const char *propertyName, int &data) override;

			private: std::string ReadPropertyValue(const char *propertyName);

				private: size_t SeekPropertyValueStartAndLength(const char *propertyName);
						 		
		public: Archive &Serialize(const char *propertyName, float &data) override;
		
		public: Archive &Serialize(const char *propertyName, char *str) override;

		
		public: bool IsInvalid() const override;
		
		public: void Close() override;
		
		public: Archive &EnterSubobject(const char *propertyName) override;
		
		public: Archive &LeaveSubobject() override;
		
	};

	
}
