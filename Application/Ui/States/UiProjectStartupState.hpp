#pragma once
#include "Ui/States/UiState.hpp"
#include "Ui/User/AssetBrowser.hpp"
#include "Ui/UiStateMachine.hpp"


namespace App::Ui::States
{
	class UiProjectStartupState final : public UiState
	{
		private: User::AssetBrowserFrontend assetBrowser;

		

		public: UiProjectStartupState(UiStateMachine &parent, const char *projectRootPath)
			:
			UiState{ parent },
			assetBrowser
			{
				projectRootPath,
				parent.GetApp()
			}
		{}
		
		public: void Update(Core::UiBuilder &builder) override;
		
	};
	
	
}