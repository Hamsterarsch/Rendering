#include "Ui/User/ShaderEditor.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "Ui/Elements/WindowElement.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Elements/InputElement.hpp"
#include "Ui/Elements/FloatLayout.hpp"
#include "Ui/Elements/ButtonElement.hpp"
#include "Ui/States/UiState.hpp"
#include "Ui/UiStateMachine.hpp"
#include "Ui/Elements/TextElement.hpp"


namespace App::Ui::User
{	
	ShaderEditorFrontend::ShaderEditorFrontend(States::UiState &parent, std::string &&shaderName, const assetSystem::AssetPtr &shaderToEdit)
		:
		parent{ &parent },
		shaderToEdit{ shaderToEdit },
		shaderName{ std::move(shaderName) },
		hasUnsavedChanges{ false },
		shouldSave{ false },
		shouldAbort{ false }
	{		
		codeToEdit.data = std::string{this->shaderToEdit->GetCode(), this->shaderToEdit->GetShaderLength() };
		auto window
		{		
			Element<WindowElement>(this->shaderName.c_str())
			->*Set{&WindowElement::size,{.75,.75}}
			->*Set
			{
				&WindowElement::tabChild,
				Element<WindowElement>("ShaderResourceNodes") << Element<TextElement>("Shader Resources")->*Set{&TextElement::pivot, {.5,0}}->*Set{&TextElement::position, {.5,0}}
			}
			->*Set{&WindowElement::tabChildSizeInPercent, .25f}
			<< (Element<FloatLayout>(5, true, true)->*Set{&FloatLayout::position, {1,1}}->*Set{&FloatLayout::pivot,{1,1}}
				<< (Element<FloatLayout>(5, true, false)->*Set{&FloatLayout::pivot,{1,1}}->*Set{&FloatLayout::position,{1, 1}}
					<< Element<ButtonElement>(*this, 0, "Save")
					<< Element<ButtonElement>(*this, 1, "Close"))
				<< Element<InputElement<Core::StringInputTarget>>(*this, 0, true, "shaderCodeInput")->*Set{&Core::UiElement::size,{1,1}})
		};
		this->window = window.get();
				
		uiElements.emplace_front(std::move(window));
		
	}



	void ShaderEditorFrontend::Update(Core::UiBuilder &builder)
	{
		if(shouldAbort)
		{
			parent->GetParent().RemoveState(*parent);
			return;
			
		}

		if(codeToEdit.ContentWasChanged())
		{
			if(not hasUnsavedChanges)
			{
				window->SetTitle((shaderName + "*").c_str());
			}
			
			hasUnsavedChanges = true;			
			codeToEdit.ClearChangedState();
		}
				
		if(not RenderAndQueryInputForUiElements(builder))
		{
			return;
			
		}

		if(shouldSave && hasUnsavedChanges)
		{
			shaderToEdit->SetCode(codeToEdit.data);
			shaderToEdit.SaveToDisk();
			
			hasUnsavedChanges = false;
			window->SetTitle(shaderName.c_str());			
		}
		
	}


	
	Core::StringInputTarget *ShaderEditorFrontend::GetInputTargetString(const size_t index)
	{
		return &codeToEdit;
		
	}


	
	bool *ShaderEditorFrontend::GetInputTargetBool(const size_t index)
	{
		switch(index)
		{
		case 0: return &shouldSave;
		case 1: return &shouldAbort;
		default: return nullptr;
		}
		
	}


}
