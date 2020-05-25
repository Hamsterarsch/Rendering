#pragma once
#include "AssetSystem/IO/Writer.hpp"
#include <filesystem>
#include <fstream>


namespace AssetSystem::IO
{
	class AssetWriter final : public Writer
	{
		private: std::fstream file;

		private: inline static const char *propertyDelimiters { ",\n" };

		private: std::string subobjectTabs;

		private: bool hasWrittenPropertyValue;

		
		
		public: AssetWriter(const std::filesystem::path &filepath);

		public: virtual ~AssetWriter() noexcept;

		
		
		public: Archive &Serialize(const char *propertyName, unsigned char *data, size_t numElements, size_t elementStrideInBytes) override;

			private: void WritePropertyName(const char *propertyName);

				private: void WritePropertyDelimiters();

			private: void WritePropertyValue(const unsigned char *data, size_t sizeInBytes, size_t elementStrideInBytes, bool isBinary = false);

		public: Archive &Serialize(const char *propertyName, int32_t &data) override;

		public: Archive &Serialize(const char *propertyName, float &data) override;

		public: Archive &Serialize(const char *propertyName, char *str) override;

		
		public: bool IsInvalid() const override;

		public: void Close() override;
						
		public: Archive &EnterSubobject(const char *propertyName) override;
		
		public: Archive &LeaveSubobject() override;
		
	};

	
}
