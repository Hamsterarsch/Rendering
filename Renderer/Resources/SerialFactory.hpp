#pragma once


namespace Renderer
{
	class SerialFactory
	{
		private: size_t currentSerial;



		public: SerialFactory();

		
		public: size_t GetNextSerial();

		public: void Reset();
				
	};
	
	
}
