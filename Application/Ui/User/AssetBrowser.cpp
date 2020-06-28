#include "Ui/User/AssetBrowser.hpp"
#include "Ui/Core/UiLayoutElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "Ui/Elements/WindowElement.hpp"
#include "Ui/Elements/ItemGridLayout.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Elements/ImageButtonElement.hpp"
#include "Ui/Elements/TextElement.hpp"
#include "Ui/Elements/ButtonElement.hpp"

#include "AssetFileending.hpp"
#include "AssetTypes/AssetTypesRegistry.hpp"
#include "AssetSystem.hpp"
#include "Ui/UiStateMachine.hpp"
#include "Ui/Elements/FloatLayout.hpp"
#include "Ui/States/UiSimpleState.hpp"
#include "AssetCreationDialog.hpp"



namespace App::Ui::User
{
	AssetBrowserFrontend::IconInfo::IconInfo(Renderer::RendererFacade &uploadTarget, assetSystem::AssetPtrTyped<Assets::ImageAsset> &&image)
		:
		image{ std::move(image) }
	{
		this->image->UploadToRenderer(uploadTarget);
		view.descriptorHandle = this->image->GetDescriptorHandle();		
	}

	AssetBrowserFrontend::Item::Item(const std::filesystem::path &path)
		:
		wasClicked{ false },
		path{ path }
	{		
	}
	

	AssetBrowserFrontend::AssetBrowserFrontend(const char *initialAbsolutePath, App::Core::Application &app)
		:
		app{ &app },
		currentPath{ std::filesystem::path{initialAbsolutePath}.parent_path() },
		rootPath{ currentPath },
		iconFolder{ app.GetRenderer(), app.GetProgramAssets().GetAsset("Images/Icons/FolderIcon.img") },
		shouldGoUp{ false },
		shouldMakeNewAsset{ false }
	{		
		auto con{ Element<ItemGridLayout>(Math::Vector2{ 100, 100 }, 5, true) };
		con->size = {1, 1};
		content = con.get();
				
		uiElements.push_front((Element<WindowElement>("Asset Browser") += std::move(con))
							  += (Element<FloatLayout>(5)
							  ->* Set{&ButtonElement::pivot, {0, 1}}
							  ->* Set{&ButtonElement::position, {.01, .99}}
							  += Element<ButtonElement>(*this, 0, "Up"))
							  += Element<ButtonElement>(*this, 1, "New")
		);
		
		DisplayCurrentPathContents();
				
	}

		void AssetBrowserFrontend::DisplayCurrentPathContents()
		{
			content->ClearChildren();
			itemList.clear();
		
			for(auto &&entry : std::filesystem::directory_iterator{ currentPath })
			{
				AddDisplay(entry);				
			}
		
		}

			void AssetBrowserFrontend::AddDisplay(const std::filesystem::directory_entry &entry)
			{
				auto absolutePath{ entry.path() };
				itemList.emplace_back(absolutePath);				

				std::string displayName;
				App::Core::ImageView icon;
				if(entry.is_directory())
				{
					displayName = relative(itemList.back().path, itemList.back().path.parent_path()).string();
					icon = iconFolder.view;
				}
				else if(entry.is_regular_file() && absolutePath.extension() == assetSystem::GetAssetFileending())
				{
					auto nameWithAssetExtension{ itemList.back().path.filename().replace_extension("") };
					auto assetExtension{ nameWithAssetExtension.extension().string() };
					assetExtension.erase(0, 1);
					
					if(app->GetAssetTypes().IsHiddenAssetType(assetExtension.c_str()))
					{
						return;
						
					}
					
					displayName = nameWithAssetExtension.replace_extension("").string();
					icon = app->GetAssetTypes().GetAssetIcon(assetExtension.c_str());
				}
				else
				{
					return;
					
				}
		
		
				content->AddChild(Element<GridLayout>(1, 3)
					+=
					{
						{ 0,0, 1,2 },
						Element<ImageButtonElement>(*this, itemList.size()+1, icon)
							->* Set{&ImageButtonElement::size, {0, 1} }
							->* Set{&ImageButtonElement::position, {.5, 0}}
							->* Set{&ImageButtonElement::pivot, {.5, 0}}
					}
					+=
					{
						{0,2, 1,1},
						Element<TextElement>(std::move(displayName))
							->* Set{&TextElement::size, {0, 1} }
							->* Set{&TextElement::position, {.5, 0} }
							->* Set{&TextElement::pivot, {.5, 0} }
					}
					
				);
		
			}

	
	
	bool *AssetBrowserFrontend::GetInputTargetBool(const size_t index)
	{
		if(index == 0)
		{
			return &shouldGoUp;
			
		}

		if(index == 1)
		{
			return &shouldMakeNewAsset;
		}
		
		return &itemList.at(index-2).wasClicked;
		
	}


	
	void AssetBrowserFrontend::Update(Core::UiBuilder &builder)
	{		
		if(shouldGoUp)
		{
			if(currentPath != rootPath)
			{
				currentPath = currentPath.parent_path();
				DisplayCurrentPathContents();				
			}			
		}
		else
		{
			ProcessLatestIconInputs();			
		}		

		
		if(not RenderAndQueryInputForUiElements(builder))
		{
			return;
			
		}

		
		if(shouldMakeNewAsset)
		{
			auto state{ MakeUnique<States::UiSimpleState>(app->GetUiStateMachine()) };
			state->SetFrontend(MakeUnique<AssetCreationDialogFrontend>(*app, currentPath.string()));
			
			app->GetUiStateMachine().PushStateLevel(std::move(state));
			return;
			
		}
		
	}

		void AssetBrowserFrontend::ProcessLatestIconInputs()
		{
			for(auto &&item : itemList)
			{
				if(item.wasClicked)
				{
					if(is_directory(item.path))
					{
						currentPath = item.path;
						DisplayCurrentPathContents();
						return;
						
					}
										
					if(auto editor{ app->GetAssetTypes().MakeAssetEditor(item.path.string().c_str(), app->GetProjectAssets().GetAsset(item.path.string().c_str())) })
					{
						app->GetUiStateMachine().AddState(std::move(editor));
					}					
					return;
					
				}
			}
		
		}

	
}
