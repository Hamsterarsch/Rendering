#include "Ui/User/AssetCreationDialog.hpp"
#include "Core/Application.hpp"
#include "Ui/Elements/ModalElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "Ui/Elements/FloatLayout.hpp"
#include "AssetTypes/AssetTypesRegistry.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Elements/ButtonElement.hpp"
#include "Ui/Elements/ImageElement.hpp"
#include "Ui/UiStateMachine.hpp"
#include "Ui/Elements/TextElement.hpp"
#include "Ui/Elements/InputElement.hpp"
#include "AssetSystem.hpp"
#include <filesystem>


namespace App::Ui::User
{
	AssetCreationDialogFrontend::AssetCreationDialogFrontend(App::Core::Application &app, std::string &&targetDirectory)
		:
		app{ &app },
		shouldAbort{ false },
		creationConfirmed{ false },
		selectedTypeIndex{ std::numeric_limits<decltype(selectedTypeIndex)>::max() },
		targetDirectory{ std::move(targetDirectory) }
	{		
		auto typesLayout{ Element<FloatLayout>(5, true) };
		for(size_t typeIndex{ 0 }; typeIndex < app.GetAssetTypes().GetNumberAssetTypes(); ++typeIndex)
		{
			if(app.GetAssetTypes().IsHiddenAssetType(typeIndex) || not app.GetAssetTypes().IsUserCreatableType(typeIndex))
			{
				continue;
				
			}						
			assetTypesStates.emplace_back(typeIndex);
			
			typesLayout->AddChild((Element<GridLayout>(4, 1)->*Set{&GridLayout::size, {1,1}}
							+= 
							{
								{0,0, 1}, Element<ImageElement>(app.GetAssetTypes().GetAssetIcon(typeIndex))
							 				->*Set{&ImageElement::size, {0, 1}}
							 				->*Set{&ImageElement::position, {.5, 0}}
							 				->*Set{&ImageElement::pivot, {.5, 0}}
							}
							+= { {1,0, 3}, Element<ButtonElement>(*this, assetTypesStates.size()+1, app.GetAssetTypes().GetAssetTypeDisplayName(typeIndex)) ->* Set{&ButtonElement::size, {1,1}} })
											->* Set{&GridLayout::size, {1, 50}}
			);
		}

		auto confirmBtn
		{
			Element<ButtonElement>(*this, 1, "Confirm") ->*Set{&ButtonElement::position, {.5,.5}} ->*Set{&ButtonElement::pivot, {.5,.5}} ->*Set{&ButtonElement::isDisabled, true}
		};
		confirmButton = confirmBtn.get();
		
		uiElements.push_front
		(
			Element<ModalElement>("Create Asset") ->*Set{&ModalElement::padding, 5.f}
			+= Element<GridLayout>(2, 6)->*Set{&GridLayout::size, {1,1}}
			+= { {0,0, 2,2}, Element<GridLayout>(3,1)->*Set{&GridLayout::size, {1,1}}
							+= {{0,0}, Element<TextElement>("Asset Name:")->*Set{&TextElement::position, {.5,.5}}->*Set{&TextElement::pivot, {.5,.5}} }
							+= {{1,0, 2}, Element<InputElement<Core::StringInputTarget>>(*this, 0, "assetNameInput")
											->*Set{&Core::UiElement::pivot,{0,.5}}
											->*Set{&Core::UiElement::position,{0,.5}}
											->*Set{&Core::UiElement::size, {1,0}}
							   }
			   }
			+= { {0,2, 2,3}, std::move(typesLayout)}
			+= { {0,5}, std::move(confirmBtn) }
			+= { {1,5}, Element<ButtonElement>(*this, 0, "Abort") ->*Set{&ButtonElement::position, {.5,.5}} ->*Set{&ButtonElement::pivot, {.5,.5}} }
		);
		
	}
	

	
	void AssetCreationDialogFrontend::Update(Core::UiBuilder &builder)
	{
		if(shouldAbort)
		{
			app->GetUiStateMachine().PopStateLevel();
			return;
			
		}		
		
		if(creationConfirmed)
		{
			if(auto importDialog{ app->GetAssetTypes().GetAssetImportDialog(selectedTypeIndex) })
			{
				app->GetUiStateMachine().PushStateLevel(std::move(importDialog));
				return;
				
			}
						
			std::filesystem::path path{ targetDirectory };
			path /= assetName.data;
			path.replace_extension(app->GetAssetTypes().GetAssetClassExtension(selectedTypeIndex));

			auto defaultInstance{ app->GetAssetTypes().GetDefaultAssetOfType(selectedTypeIndex) };
			
			app->GetProjectAssets().MakeAsset(path.string().c_str(), std::move(*defaultInstance));
			app->GetUiStateMachine().PopStateLevel();
			return;
												
		}
		
		if(not RenderAndQueryInputForUiElements(builder))
		{
			return;
		}

		for(auto &&state : assetTypesStates)
		{
			if(state.wasClicked)
			{
				selectedTypeIndex = state.typeIndex;
				TryToEnableAssetCreation();
				break;
			}
		}

		if(assetName.ContentWasChanged())
		{
			TryToEnableAssetCreation();		
			assetName.ClearChangedState();
		}
				
	}

		void AssetCreationDialogFrontend::TryToEnableAssetCreation()
		{
			confirmButton->isDisabled = assetName.data.empty() || selectedTypeIndex == std::numeric_limits<decltype(selectedTypeIndex)>::max();
					
		}



	bool *AssetCreationDialogFrontend::GetInputTargetBool(const size_t index)
	{
		if(index == 0)
		{
			return &shouldAbort;	
		}
		else if(index == 1)
		{
			return &creationConfirmed;
		}
		else
		{
			return &assetTypesStates.at(index-2).wasClicked;
		}
				
	}


	
	Core::StringInputTarget *AssetCreationDialogFrontend::GetInputTargetString(const size_t index)
	{
		return &assetName;
		
	}

	
}
