#pragma once
#include "Archivable.hpp"


namespace App::Core
{
	class Version final : public assetSystem::Archivable
	{		
		public: int major{ 0 };

		public: int minor{ 0 };

		public: int patch{ 0 };


		
		public: Version(int major, int minor, int patch) :
			major{ major },
			minor{ minor },
			patch{ patch }
		{}
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
						
	
	};

	inline bool operator==(const Version &lhs, const Version &rhs)
	{
		return !(lhs.major != rhs.major || lhs.minor != rhs.minor || lhs.patch != rhs.patch);
		
	}

	inline bool operator!=(const Version &lhs, const Version &rhs)
	{
		return !(lhs == rhs);
		
	}
	
	
}
