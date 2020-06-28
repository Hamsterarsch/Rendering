#pragma once


namespace App::Ui::Filter
{
	class SerializationFilter
	{
		public: static bool CharacterIsForbidden(char character)
		{
			return character == '`';
			
		}
		
	};

	
}
