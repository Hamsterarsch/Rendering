#pragma once
#include "AssetSystem/IO/Archive.hpp"
#include <string>


namespace AssetSystem::IO
{
	class AssetArchiveTests
	{
		private: struct TestDataNested : public Archivable
		{
			float val1;
			int val2;
			
			Archive &Serialize(Archive &archive) override;
			
		};
		
		private: struct TestData : public Archivable
		{
			int number;		
			int number2;
			std::string text;		
			float val1;
			float val2;

			TestDataNested nested;
			
			unsigned blob[3]{};

			Archive &Serialize(Archive &archive) override;

			bool operator==(const TestData &rhs) const;
			
		};

		public: static void Test();		
		
	};
	
	
}
