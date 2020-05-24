#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include <cstdint>


namespace AssetSystem::IO
{
	
	class Archive
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Archive)
					
		
		public: virtual Archive &SerializeBlob(const char *propertyName, unsigned char *data, size_t sizeInBytes) = 0;
						
		public: virtual Archive &SerializeUint32(const char *propertyName, unsigned char *data) = 0;

		public: virtual Archive &SerializeInt32(const char *propertyName, unsigned char *data) = 0;

		public: virtual Archive &SerializeFloat(const char *propertyName, unsigned char *data) = 0;

		public: virtual Archive &SerializeString(const char *propertyName, char *str) = 0;
		

		public: virtual bool IsWriting() const = 0;

		public: virtual bool IsReading() const = 0;

		public: virtual bool IsInvalid() const = 0;

		public: virtual void Close() = 0;

		public: virtual Archive &EnterSubobject(const char *propertyName) = 0;

		public: virtual Archive &LeaveSubobject() = 0;
		
	};


	
}
