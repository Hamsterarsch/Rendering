#pragma once
#include "UiState.hpp"
#include <string>
#include "AssetPtr.hpp"


namespace App::Ui::States
{
	class UiEditorState : public UiState
	{
		protected: std::string assetName;

		protected: assetSystem::AssetPtr targetAsset;
				   

		public: UiEditorState(UiStateMachine &parent, std::string &&assetName, const assetSystem::AssetPtr &assetToEdit)
			:
			UiState{ parent },
			assetName{ std::move(assetName) },
			targetAsset{ assetToEdit }
		{}		
		
	};
	
	
}
