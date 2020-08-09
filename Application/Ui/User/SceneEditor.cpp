#include "Ui/User/SceneEditor.hpp"
#include "Ui/Misc/OutlinerInfo.hpp"
#include "Ui/Elements/WindowElement.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "Ui/Elements/FloatLayout.hpp"
#include "Ui/Elements/TextElement.hpp"
#include "Scene/SceneGraph.hpp"
#include "Ui/Elements/ButtonElement.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "AssetSystem.hpp"
#include "AssetTypes/StaticMeshAsset.hpp"
#include "AssetTypes/PipelineAsset.hpp"
#include "Scene/ContentMesh.hpp"
#include "Scene/ContentLight.hpp"
#include "Ui/Elements/SpacingElement.hpp"


namespace App::Ui::User
{
	SceneEditorFrontend::SceneEditorFrontend(App::Core::Application &app)
		:
		app{ &app }		
	{
		auto contentEditorLayout{ Element<FloatLayout>(1, true) };
		objectEditorLayout = contentEditorLayout.get();
		
		auto window
		{
			Element<WindowElement>("Scene Editor")
			->*Set{&WindowElement::isVerticalTabChild, true}
			->*Set{&WindowElement::tabChildSizeInPercent, .6f}
			->*Set{&WindowElement::size, {.25, 1}}
			->*Set{&WindowElement::tabChild, Element<WindowElement>("ObjectEditorWindow")<<(Element<FloatLayout>(5, true)<<Element<TextElement>("Object Editor")<<std::move(contentEditorLayout))}
			->*Set{&WindowElement::pivot, {1, 0}}
			->*Set{&WindowElement::position, { 1, 0 }}
		};

		auto outlinerLayout{ Element<FloatLayout>(1, true) };
		outlinerRootLayout = outlinerLayout.get();

		auto selectionText{ Element<TextElement>("nothing selected") };
		selectionDisplay = selectionText.get();
		
		window << 
			(
				Element<FloatLayout>(5, true)
				<< 
				(		
					Element<GridLayout>(2, 1, 1)->*Set{&GridLayout::size, {1,0}}
					+= { {0,0}, Element<ButtonElement>(*this, 0, "AddCube")->*Set{&ButtonElement::size, {1, 0}} }
					+= { {1,0}, Element<ButtonElement>(*this, 1, "AddLight")->*Set{&ButtonElement::size, {1, 0}} }
				)
				<< std::move(selectionText)
				<< Element<TextElement>("Outliner") 
				<< std::move(outlinerLayout)
			);
				
		uiElements.push_front(std::move(window));			

		OnSceneGraphChanged();

	}

		void SceneEditorFrontend::OnSceneGraphChanged()
		{
			app->GetCurrentScene().Accept(outlinerDataHarvester);
			UpdateOutlinerContents(outlinerDataHarvester.root);
		}

			void SceneEditorFrontend::UpdateOutlinerContents(const OutlinerInfo &outlinerGraphRoot)
			{
				entryButtonStates.clear();
				outlinerRootLayout->ClearChildren();
				outlinerFrontends.clear();
				
				PushNodeEntryToLayout(outlinerGraphRoot, *outlinerRootLayout, true);
				
			}

				void SceneEditorFrontend::PushNodeEntryToLayout(const OutlinerInfo &nodeInfo, Core::UiLayoutElement &layout, const bool isRootNode)
				{
					if(nodeInfo.children.empty())
					{
						if(isRootNode)
						{
							layout.AddChild(Element<TextElement>(nodeInfo.node->GetDisplayName())->*Set{&ButtonElement::size, {1,0}});
							return;
							
						}
						entryButtonStates.push_back({false});
						layout.AddChild(Element<ButtonElement>(*this, entryButtonStates.size()+1, nodeInfo.node->GetDisplayName())->*Set{&ButtonElement::size, {1,0}});
						return;

					}
								
					outlinerFrontends.emplace_back(MakeUnique<CollapsibleNodeLayoutFrontend>());
					auto subLayout{ Element<FloatLayout>(1, true) };
				
					for( uint32_t childIndex{ 0 }; childIndex < nodeInfo.children.size(); ++childIndex)
					{				
						PushNodeEntryToLayout(nodeInfo.children.at(childIndex), *subLayout, false);							
					}

					layout.AddChild( Element<CollapsibleNodeLayout>(*outlinerFrontends.back(), nodeInfo.node->GetDisplayName()) << std::move(subLayout));
			
				}

	

	void SceneEditorFrontend::Update(Core::UiBuilder &builder)
	{
		if(not RenderAndQueryInputForUiElements(builder))
		{
			return;
			
		}

		for(auto &&frontend : outlinerFrontends)
		{
			frontend->Update(builder);
		}

		for(uint32_t index{ 0 }; index < entryButtonStates.size(); ++index)
		{
			if(entryButtonStates.at(index).value)
			{
				selectionDisplay->SetText(outlinerDataHarvester.root.children.at(index).node->GetDisplayName());
				objectEditorLayout->ClearChildren();

				auto &node{ outlinerDataHarvester.root.children.at(index).node };
				currentTransformEditorFrontend = node->GetTransformEditor(*objectEditorLayout);
				
				if(auto *content{ node->GetContent() })
				{
					currentObjectEditorFrontend = content->MakeContentEditor(*objectEditorLayout);									
				}
				break;				
			}			
		}

		if(currentObjectEditorFrontend)
		{
			currentObjectEditorFrontend->Update(builder);
		}

		if(currentTransformEditorFrontend)
		{
			currentTransformEditorFrontend->Update(builder);
		}

		if(shouldAddCube)
		{
			static uint32_t s{ 0 };
			assetSystem::AssetPtrTyped<Assets::StaticMeshAsset> mesh{ app->GetProgramAssets().GetAsset("Meshes/MetricCube.msh") };
			assetSystem::AssetPtrTyped<Assets::PipelineAsset> pipeline{ app->GetProgramAssets().GetAsset("DefaultAssets/DefaultPipelineState.pso")};
			
			outlinerDataHarvester.root.node->AddChild((std::to_string(++s)+"_Cube").data(), {}, MakeUnique<Scene::ContentMesh>(mesh, pipeline));
			OnSceneGraphChanged();
			return;
			
		}

		if(shouldAddLight)
		{
			static uint32_t s{ 0 };
			outlinerDataHarvester.root.node->AddChild((std::to_string(++s)+"_Light").data(), {}, MakeUnique<Scene::ContentLight>(1, Math::Vector3{1,1,1}, 0, 0));
			OnSceneGraphChanged();
			return;
			
		}
		
	}

	bool *SceneEditorFrontend::GetInputTargetBool(const size_t index)
	{
		switch(index)
		{
		case 0: return &shouldAddCube;
		case 1: return &shouldAddLight;
		default: return &entryButtonStates.at(index-2).value;
		}
		
	}

	
}
