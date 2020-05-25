#pragma once


namespace AssetSystem::IO
{
	struct AssetArchiveConstants
	{
		public: static inline constexpr char binaryStartToken[8]{ "____bin" };

		public: static inline constexpr char binaryEndToken[8]{ "ary____" };

		public: static inline constexpr char binaryTokenLength{ 8 };
		
	};
	
}
