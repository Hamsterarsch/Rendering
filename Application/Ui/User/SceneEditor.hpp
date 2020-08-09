#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Ui/User/ObjectList.hpp"
#include "Core/Application.hpp"
#include "Ui/Misc/OutlinerInfo.hpp"
#include "Ui/Misc/GraphVisitorOutlinerInfo.hpp"


namespace App::Ui
{
	struct OutlinerInfo;
	class FloatLayout;
	class TextElement;
}
namespace App::Ui::Core { class UiLayoutElement; }


namespace App::Ui::User
{
	struct BoolBucket{ bool value; };
	
	class SceneEditorFrontend final : public Core::UiFrontend
	{
		private: FloatLayout *outlinerRootLayout;

		private: std::vector<UniquePtr<CollapsibleNodeLayoutFrontend>> outlinerFrontends;

		private: App::Core::Application *app;

		private: GraphVisitorOutlinerInfo outlinerDataHarvester;

		private: TextElement *selectionDisplay;

		private: std::vector<BoolBucket> entryButtonStates;

		private: bool shouldAddCube;

		private: bool shouldAddLight;
				 		
		private: FloatLayout *objectEditorLayout;
				 
		private: UniquePtr<UiFrontend> currentTransformEditorFrontend;
		
		private: UniquePtr<UiFrontend> currentObjectEditorFrontend;
		
		
		
		public: SceneEditorFrontend(App::Core::Application &app);

			private: void OnSceneGraphChanged();
		
				private: void UpdateOutlinerContents(const OutlinerInfo &outlinerGraphRoot);

					private: void PushNodeEntryToLayout(const OutlinerInfo &nodeInfo, Core::UiLayoutElement &layout, bool isRootNode);

		
		public: void Update(Core::UiBuilder &builder) override;

		public: bool *GetInputTargetBool(size_t index) override;
		
	};

	
}