#include "Ui/User/AssetBrowser.hpp"
#include "Ui/Elements/WindowElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"

namespace App::Ui::User
{
	AssetBrowserFrontend::AssetBrowserFrontend(const char *initialAbsolutePath) : currentPath{ initialAbsolutePath }
	{
		uiElements.push_front(Element<WindowElement>("Asset Browser"));
		
		DisplayCurrentPathContents();

		
	}

		void AssetBrowserFrontend::DisplayCurrentPathContents()
		{
			for(auto &&entry : std::filesystem::directory_iterator{ currentPath })
			{
				if(entry.is_regular_file())
				{
					AddFileDisplay(entry.path());
					continue;
					
				}

				if(entry.is_directory())
				{
					AddFolderDisplay(entry.path());
					continue;
					
				}				
			}
		
		}

			void AssetBrowserFrontend::AddFolderDisplay(const std::filesystem::path &absolutePath)
			{
			}

			void AssetBrowserFrontend::AddFileDisplay(const std::filesystem::path &absolutePath)
			{
			}

	
}
