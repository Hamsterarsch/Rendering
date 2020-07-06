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
#include "Ui/Elements/CollapsibleNodeLayout.hpp"
#include "Ui/Elements/SpacingElement.hpp"


namespace App::Ui::User
{	
	unsigned *ShaderConstantsSlotFrontend::GetInputTargetUnsigned(const size_t index)
	{
		if(index == 0)
		{
			return &slot.shaderRegister;
		}
		
		if(index == 1)
		{
			return &slot.sizeInVector4;
		}
		
		return nullptr;
		
	}


	
	UniquePtr<Core::UiElement> ShaderConstantsSlotFrontend::MakeEditUi()
	{
		return Element<FloatLayout>(5, true)
				<< Element<TextElement>("Shader Register:")
				<< Element<InputElement<unsigned>>(*this, 0)->*Set{&InputElementBase::size, {1,0}}
				<< Element<SpacingElement>(Math::Vector2{0,5})
				<< Element<TextElement>("Num Vector 4's:")
				<< Element<InputElement<unsigned>>(*this, 1)->*Set{&InputElementBase::size, {1,0}};
		
	}


	

	unsigned *TextureSlotFrontend::GetInputTargetUnsigned(const size_t index)
	{
		return &slot.shaderRegister;
		
	}


	
	UniquePtr<Core::UiElement> TextureSlotFrontend::MakeEditUi()
	{
		return Element<FloatLayout>(5, true)
				<< Element<TextElement>("Shader Register:")
				<< Element<InputElement<unsigned>>(*this, 0)->*Set{&InputElementBase::size, {1,0}};
		
	}





	ShaderEditorFrontend::ShaderEditorFrontend(States::UiState &parent, std::string &&shaderName, const assetSystem::AssetPtr &shaderToEdit)
		:
		parent{ &parent },
		editingTarget{ shaderToEdit },
		shaderName{ std::move(shaderName) },
		window{ nullptr },
		shouldSave{ false },
		shouldAbort{ false },
		shouldAddTextureResource{ false },
		shouldRemoveTextureResource{ false },
		shouldAddCbvResource{ false },
		shouldRemoveCbvResource{ false }
	{		
		codeToEdit.data = std::string{editingTarget->GetCode(), editingTarget->GetCodeLength() };
		
		auto textureSlotItems
		{
			Element<FloatLayout>(5, true) 
				<< (Element<GridLayout>(2,1, 1)->*Set{&GridLayout::size, {1,0}}
					+= {{0,0}, Element<ButtonElement>(*this, 2, "Add" )->*Set{&ButtonElement::size, {1,0}}} 
					+= {{1,0}, Element<ButtonElement>(*this, 3, "Remove")->*Set{&ButtonElement::size, {1,0}}}
				   )			
		};
		textureSlotList = ObjectListFrontend{ *textureSlotItems };
		
		for(size_t slotIndex{ 0 }; slotIndex < editingTarget->GetNumTextureSlots(); ++slotIndex)
		{
			PushTextureSlotUi(editingTarget->GetTextureSlotAt(slotIndex));
		}
		
		
		auto cbvSlotItems
		{
			(Element<FloatLayout>(5, true) 
				<< (Element<GridLayout>(2,1, 1)->*Set{&GridLayout::size, {1,0}} 
						+= {{0,0}, Element<ButtonElement>(*this, 4, "Add" )->*Set{&ButtonElement::size, {1,0}}} 
						+= {{1,0}, Element<ButtonElement>(*this, 5, "Remove")->*Set{&ButtonElement::size, {1,0}}}
				   )
			)
		};
		constantsSlotList = ObjectListFrontend{ *cbvSlotItems };
		
		for(size_t slotIndex{ 0 }; slotIndex < editingTarget->GetNumConstantsSlots(); ++slotIndex)
		{
			PushConstantsSlotUi(editingTarget->GetConstantsSlotAt(slotIndex));
		}
		
		
		auto window
		{		
			Element<WindowElement>(this->shaderName.c_str())
			->*Set{&WindowElement::size,{.75,.75}}
			->*Set
			{
				&WindowElement::tabChild,
				Element<WindowElement>("ShaderResourceNodes")
				<<	(Element<FloatLayout>(5, true) 
						<< (Element<CollapsibleNodeLayout>(textureSlotsNodeFrontend, "Texture Resources") << std::move(textureSlotItems))
						<< (Element<CollapsibleNodeLayout>(constantSlotsNodeFrontend, "Cbv Resources") << std::move(cbvSlotItems))
					)
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


	
	void ShaderEditorFrontend::PushTextureSlotUi(const Assets::TextureResourceSlot &slot)
	{
		auto slotFrontend{ MakeUnique<t_textureSlotFrontend>(slot) };
		auto editUi{ slotFrontend->MakeEditUi() };
		textureSlotList.PushBack(("Texture Slot "+std::to_string(textureSlotList.GetNumEntries())).c_str(), std::move(editUi), std::move(slotFrontend));
		
	}


	
	void ShaderEditorFrontend::PushConstantsSlotUi(const Assets::ConstantsResourceSlot &slot)
	{
		auto slotFrontend{ MakeUnique<t_constantsSlotFrontend>(slot) };
		auto editUi{ slotFrontend->MakeEditUi() };
		constantsSlotList.PushBack(("Constants Slot "+std::to_string(constantsSlotList.GetNumEntries())).c_str(), std::move(editUi), std::move(slotFrontend));
		
	}



	void ShaderEditorFrontend::Update(Core::UiBuilder &builder)
	{
		if(shouldAbort)
		{
			parent->GetParent().RemoveState(*parent);
			return;
			
		}
				
		if(not RenderAndQueryInputForUiElements(builder))
		{
			return;
			
		}
			   
		HandleResourceSlotUpdates(builder);
						
		if(shouldSave)
		{
			SaveChanges();									
		}
		
	}

		void ShaderEditorFrontend::HandleResourceSlotUpdates(Core::UiBuilder &builder)
		{
			HandleTextureSlotUpdates(builder);
			HandleConstantsSlotUpdates(builder);
		
		}

			void ShaderEditorFrontend::HandleTextureSlotUpdates(Core::UiBuilder &builder)
			{
				textureSlotsNodeFrontend.Update(builder);
				if(shouldAddTextureResource)
				{
					PushTextureSlotUi({});
				}

				if(shouldRemoveTextureResource)
				{
					textureSlotList.PopBack();			
				}
				textureSlotList.Update(builder);
						
			}

			void ShaderEditorFrontend::HandleConstantsSlotUpdates(Core::UiBuilder &builder)
			{
				constantSlotsNodeFrontend.Update(builder);		
				if(shouldAddCbvResource)
				{
					PushConstantsSlotUi({});		
				}

				if(shouldRemoveCbvResource)
				{
					constantsSlotList.PopBack();
				}
				constantsSlotList.Update(builder);
		
			}

		void ShaderEditorFrontend::SaveChanges()
		{
			editingTarget->SetCode(codeToEdit.data);
		
			editingTarget->ClearResourceSlots();			
			textureSlotList.ForEachFrontend([&shader = editingTarget](UiFrontend &frontend)
			{
				shader->AddTextureSlot(reinterpret_cast<t_textureSlotFrontend &>(frontend).GetSlot());
			});

			constantsSlotList.ForEachFrontend([&shader = editingTarget](UiFrontend &frontend)
			{
				shader->AddConstantsSlot(reinterpret_cast<t_constantsSlotFrontend &>(frontend).GetSlot());
			});
			
			editingTarget.SaveToDisk();
		
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
		case 2: return &shouldAddTextureResource;
		case 3: return &shouldRemoveTextureResource;
		case 4: return &shouldAddCbvResource;
		case 5: return &shouldRemoveCbvResource;
		default: return nullptr;
		}
		
	}


}
