#include "AssetSystem/IO/Filetypes/Test/AssetArchiveTests.hpp"
#include "AssetSystem/IO/Filetypes/AssetWriter.hpp"
#include "AssetSystem/IO/Filetypes/AssetReader.hpp"
#include <cassert>


namespace assetSystem::io
{
	Archive &AssetArchiveTests::TestDataNested::Serialize(Archive &archive)
	{
		return archive.Serialize("val1", val1).Serialize("val2", val2);
				
	}

	Archive &AssetArchiveTests::TestData::Serialize(Archive &archive)
	{
		archive
		.Serialize("number", number)
		.Serialize("number2", number2)
		.EnterSubobject("text");

		auto textSize{ static_cast<int>(text.size()) };
		archive.Serialize("size", textSize);

		text.resize(textSize);		
		archive.Serialize("str", text.data())
		
		.LeaveSubobject()
		.Serialize("val1", val1)
		.Serialize("val2", val2)
		.Serialize("nested", nested);
		archive.Serialize("blob", reinterpret_cast<unsigned char *>(blob), 3, sizeof(std::remove_all_extents_t<decltype(blob)>));

		return archive;
		
	}

	bool AssetArchiveTests::TestData::operator==(const TestData &rhs) const
	{
		return
		number == rhs.number
		&& number2 == rhs.number2
		&& text == rhs.text
		&& val1 == rhs.val1
		&& val2 == rhs.val2
		&& nested.val1 == rhs.nested.val1
		&& nested.val2 == rhs.nested.val2
		&& blob[0] == rhs.blob[0]
		&& blob[1] == rhs.blob[1]
		&& blob[2] == rhs.blob[2];
		
	}

	void AssetArchiveTests::Test()
	{
		AssetWriter writer{ "testAsset.asset" };

		TestData data{};
		data.number = 10;
		data.number2 = 42;
		data.text = "zoom... sharpen.";
		data.val1 = 0.5;
		data.val2 = 10.11;

		data.nested.val1 = 99.99;
		data.nested.val2 = -18;

		data.blob[0] = 12;
		data.blob[1] = 18;
		data.blob[2] = 432;

		writer.Serialize("testData", data);
		writer.Close();

		TestData readData{};
		AssetReader reader{ "testAsset.asset" };
		reader.Serialize("testData", readData);
		reader.Close();
				
		assert(data == readData);
		
	}
	
}
