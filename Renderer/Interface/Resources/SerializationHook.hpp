#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "RendererExportHelper.hpp"

namespace Renderer
{
	class RENDERER_DLLSPEC SerializationHook
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(SerializationHook)

		
		private: class Block
		{
			private: SerializationHook *parent;

			
			friend SerializationHook;
			
			public:	explicit Block(SerializationHook *parent);

			public: Block(const Block &) = delete;
			
				public: Block(Block &&other) noexcept;
								
			public: ~Block();
			
		};

		private: size_t lastBlockSizeInBytes{ 0 };


		friend Block::~Block();
		
		public: virtual void WriteToBlock(const void *data, size_t sizeInBytes) {}
		
		private: virtual void EndBlock() {}
		
		protected: virtual void OnBeginBlock() = 0;
		
		protected: size_t GetBlockSize() const {  return lastBlockSizeInBytes; }

		
		public: Block BeginBlock(size_t sizeInBytes);
		
	};
	
}
