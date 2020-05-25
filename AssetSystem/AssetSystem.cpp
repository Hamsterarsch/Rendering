#include "AssetSystem.hpp"


#include "AssetSystem/IO/Filetypes/AssetWriter.hpp"
#include "Shared/Filesystem/Conversions.hpp"
#include "IO/Filetypes/AssetReader.hpp"
#include "IO/DiskConversions.hpp"

namespace AssetSystem
{
	struct testData
	{
		int number;		
		std::string text;		
		float floating;

		int subNumber;

		float subsubFloat;
		
		unsigned blob[3]{};
	};

	void Serialize(IO::Archive *archive, testData &data)
	{		
		(*archive)
		.Serialize("blob", reinterpret_cast<unsigned char *>(data.blob), ARRAYSIZE(data.blob), sizeof(std::remove_all_extents_t<decltype(data.blob)>))
		.Serialize("number", data.number)
		.Serialize("text", data.text.data())
		.EnterSubobject("subobj")
			.Serialize("subNumber", data.subNumber)
			.EnterSubobject("sub2")
				.Serialize("subFloat", data.subsubFloat)
			.LeaveSubobject()
		.LeaveSubobject()
		.Serialize("float", data.floating);
		
	}
	
	UniquePtr<AssetSystem> GetAssetSystem()
	{
		const auto path{ Filesystem::Conversions::MakeExeRelative("../../Assets/Test/") };
		std::filesystem::create_directories(path);

		auto e = IO::IsBigEndianMachine();

		testData test;
		test.number = -1123;
		test.floating = 10.11;
		test.text = "nice asset format";
		test.blob[0] = 1111111111;
		test.blob[1] = 665;
		test.blob[2] = 1231231232;
		test.subNumber = 23;
		test.subsubFloat = 0.5;
		IO::AssetWriter writer{ path + "/test.asset" };

		Serialize(&writer, test);
		writer.Close();

		//do not write empty objects pls
		//endian conversion

		
		testData read;
		IO::AssetReader reader{ path + "/test.asset" };
		Serialize(&reader , read);
		
		return {};
	}

	

	
}
