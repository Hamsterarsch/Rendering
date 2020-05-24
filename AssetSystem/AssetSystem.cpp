#include "AssetSystem.hpp"


#include "AssetSystem/IO/Filetypes/AssetWriter.hpp"
#include "Shared/Filesystem/Conversions.hpp"

namespace AssetSystem
{
	UniquePtr<AssetSystem> GetAssetSystem()
	{
		const auto path{ Filesystem::Conversions::MakeExeRelative("../../Assets/Test/") };
		std::filesystem::create_directories(path);
		
		IO::AssetWriter writer{ path + "/test.asset" };

		unsigned r{ 24 };
		int e{ -54 };
		std::string s{ "eine gans ging nach gassau !" };		
		float ff{ 4.2 };
		unsigned blob[]{2345,76543,3456};
		
		writer
		.SerializeUint32("integerProp", reinterpret_cast<unsigned char* >(&r))
		.SerializeInt32("negIntegerProp", reinterpret_cast<unsigned char *>(&e))
		.SerializeString("text", s.data())
		.SerializeFloat("floap", reinterpret_cast<unsigned char *>(&ff))
		.SerializeBlob("blob", reinterpret_cast<unsigned char *>(blob), ARRAYSIZE(blob) * sizeof(unsigned))
		
		.EnterSubobject("object1")
			.SerializeInt32("objn", reinterpret_cast<unsigned char *>(&e))
		.LeaveSubobject()
		
		.EnterSubobject("object1")
			.SerializeInt32("objn", reinterpret_cast<unsigned char *>(&e))
			.EnterSubobject("emptyo")
		.LeaveSubobject()
		
		.EnterSubobject("lastofobject")
			.SerializeInt32("into", reinterpret_cast<unsigned char*>(&e))
		.Close();
		
		return {};
	}

	
}
