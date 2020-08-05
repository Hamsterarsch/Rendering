#pragma once
#include "Ui/States/UiState.hpp"
#include "Ui/User/AssetBrowser.hpp"
#include "Ui/UiStateMachine.hpp"
#include "Ui/User/SceneEditor.hpp"



namespace App::Ui::States
{
	class UiProjectStartupState final : public UiState
	{
		private: User::AssetBrowserFrontend assetBrowser;

		private: User::SceneEditorFrontend sceneEditor;
		

		
		public: UiProjectStartupState(UiStateMachine &parent, const char *projectRootPath)
			:
			UiState{ parent },
			assetBrowser
			{
				projectRootPath,
				parent.GetApp()
			},
			sceneEditor{ parent.GetApp() }
		{}
		
		public: void Update(Core::UiBuilder &builder) override;
		
	};
	
	
}
