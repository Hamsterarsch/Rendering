#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include <vector>
#include "StringInputTargetImpl.hpp"
#include "Ui/Misc/AssetNameFilter.hpp"
#include <functional>


namespace App::Ui { class ButtonElement; }
namespace App::Core{ class Application; }


namespace App::Ui::User
{
	class AssetCreationDialogFrontend final : public Core::UiFrontend
	{
		private: App::Core::Application *app;

		private: bool shouldAbort;

		private: bool creationConfirmed;

		private: size_t selectedTypeIndex;

		private: std::string targetDirectory;

		private: struct AssetTypeSelected
		{
			size_t typeIndex;
			bool wasClicked;

			AssetTypeSelected(size_t typeIndex) : typeIndex{ typeIndex }, wasClicked{ false } {}
			
		};
		
		private: std::vector<AssetTypeSelected> assetTypesStates;

		private: StringInputTargetImpl<Filter::AssetNameFilter> assetName;

		private: ButtonElement *confirmButton;

		private: std::function<void()> onAssetCreated;
		
		
		
		public: AssetCreationDialogFrontend(App::Core::Application &app, std::string &&targetDirectory, std::function<void()> &&onAssetCreated);
		
		public: void Update(Core::UiBuilder& builder) override;

			private: void TryToEnableAssetCreation();
		
		public: bool *GetInputTargetBool(size_t index) override;

		Core::StringInputTarget *GetInputTargetString(size_t index) override;
		
	};


}
