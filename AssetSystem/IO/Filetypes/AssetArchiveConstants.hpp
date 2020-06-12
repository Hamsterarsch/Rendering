#pragma once


namespace assetSystem::IO
{
	struct AssetArchiveConstants
	{
		public: static inline constexpr const char *binaryStartToken{ "____bin" };

		public: static inline constexpr const char *binaryEndToken{ "ary____" };

		public: static inline constexpr char binaryTokenLength{ 8 };
		
	};
	
}
