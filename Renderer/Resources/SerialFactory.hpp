#pragma once


namespace Renderer
{
	class SerialFactory
	{
		private: size_t currentSerial;



		SerialFactory();

		
		public: size_t GetNextSerial();

		public: void Reset();
				
	};
	
	
}
