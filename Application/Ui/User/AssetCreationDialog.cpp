#include "Ui/User/AssetCreationDialog.hpp"
#include "Core/Application.hpp"
#include "Ui/Elements/ModalElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "Ui/Elements/FloatLayout.hpp"
#include "AssetTypes/AssetTypesRegistry.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Elements/ButtonElement.hpp"


namespace App::Ui::User
{
	AssetCreationDialogFrontend::AssetCreationDialogFrontend(App::Core::Application &app)
		:
		app{ &app }
	{		
		auto layout{ Element<FloatLayout>(5, true) };
		for(size_t typeIndex{ 0 }; typeIndex < app.GetAssetTypes().GetNumberAssetTypes(); ++typeIndex)
		{
			layout->AddChild((Element<GridLayout>(4, 1) 
							 += { {1,0, 3}, Element<ButtonElement>(*this, 0, app.GetAssetTypes().GetAssetTypeDisplayName(typeIndex)) ->* Set{&ButtonElement::size, {1,1}} })
							->* Set{&GridLayout::size, {1, 50}}
			);
		}
		
		uiElements.push_front(Element<ModalElement>("Select Asset Type") ->*Set{&ModalElement::padding, 5.f} += std::move(layout));
		
	}
	

	
	void AssetCreationDialogFrontend::Update(Core::UiBuilder &builder)
	{
		RenderAndQueryInputForUiElements(builder);
				
	}

	
}
