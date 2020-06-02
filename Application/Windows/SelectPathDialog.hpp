#pragma once
#include <filesystem>


namespace App::Windows
{
	class SelectPathDialog
	{
		private: std::filesystem::path selectedItem;
		
		
		public: SelectPathDialog();
				
		public: std::filesystem::path GetSelectedItem() const { return selectedItem; }
		
	};
	
	
}
