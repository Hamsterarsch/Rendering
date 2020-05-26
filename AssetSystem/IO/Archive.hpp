#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include <cstdint>


namespace AssetSystem::IO
{
	class Archivable;
	
	class Archive
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Archive)
					
		
		public: virtual Archive &Serialize(const char *propertyName, unsigned char *data, size_t numElements, size_t elementStrideInBytes) = 0;

		public: virtual Archive &Serialize(const char *propertyName, int32_t &data) = 0;

		public: virtual Archive &Serialize(const char *propertyName, float &data) = 0;
				
		public: virtual Archive &Serialize(const char *propertyName, char *str) = 0;

		public: virtual Archive &Serialize(const char *propertyName, Archivable &archivable) = 0;

				

		public: virtual bool IsWriting() const = 0;

		public: virtual bool IsReading() const = 0;

		public: virtual bool IsInvalid() const = 0;

		public: virtual void Close() = 0;

		public: virtual Archive &EnterSubobject(const char *propertyName) = 0;

		public: virtual Archive &LeaveSubobject() = 0;
		
	};


	
	class Archivable
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Archivable)
					
		public: virtual Archive &Serialize(Archive &archive) = 0;
				
	};


	class ArchiveBase : public Archive
	{
		public: Archive &Serialize(const char *propertyName, Archivable &archivable) final override;
		
	};
	
	
}
