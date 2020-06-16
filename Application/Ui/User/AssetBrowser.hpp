#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include <filesystem>


namespace App::Ui::User
{
	class AssetBrowserFrontend final : public Core::UiFrontend
	{
		private: std::filesystem::path currentPath;

		
		
		public: AssetBrowserFrontend(const char *initialAbsolutePath);

			private: void DisplayCurrentPathContents();

				private: void AddFolderDisplay(const std::filesystem::path &absolutePath);

				private: void AddFileDisplay(const std::filesystem::path &absolutePath);
		
	};
	
	
}
