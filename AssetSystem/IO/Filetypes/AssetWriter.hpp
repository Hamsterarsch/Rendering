#pragma once
#include "AssetSystem/IO/Writer.hpp"
#include <filesystem>
#include <fstream>
#include "AssetSystemTypes.hpp"
#include <functional>


namespace assetSystem::io
{
	class AssetWriter final : public Writer
	{
		private: std::fstream file;

		private: inline static const char *propertyDelimiters { ",\n" };

		private: std::string subobjectTabs;

		private: bool hasWrittenPropertyValue;

		private: std::function<void(AssetKey)> onAssetPtr;

		
		
		public: AssetWriter(const std::filesystem::path &filepath, std::function<void(AssetKey)> &&onAssetPtr);

		public: virtual ~AssetWriter() noexcept;


		public: using ArchiveBase::Serialize;
		
		public: Archive &Serialize(const char *propertyName, unsigned char *data, size_t numElements, size_t elementStrideInBytes) override;

			private: void WritePropertyName(const char *propertyName);

				private: void WritePropertyDelimiters();

			private: void WritePropertyValue(const unsigned char *data, size_t sizeInBytes, size_t elementStrideInBytes, bool isBinary = false);

				private: void WriteFromLittleEndian(const unsigned char *data, size_t numElements, size_t elementStrideInBytes);

		public: Archive &Serialize(const char *propertyName, int32_t &data) override;

		public: Archive &Serialize(const char *propertyName, uint32_t &data) override;
		
		public: Archive &Serialize(const char *propertyName, float &data) override;

		public: Archive &Serialize(const char *propertyName, char *str) override;

		public: Archive &Serialize(const char *propertyName, AssetPtr &asset) override;
		
		public: size_t GetPropertySizeInBytes(const char *propertyName) override { return 0; }
		
		
		public: bool IsInvalid() const override;

		public: void Close() override;
						
		public: Archive &EnterSubobject(const char *propertyName) override;
		
		public: Archive &LeaveSubobject() override;

	};

	
}
