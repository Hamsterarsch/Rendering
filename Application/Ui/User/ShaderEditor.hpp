#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "AssetTypes/ShaderAsset.hpp"
#include "AssetPtr.hpp"
#include "StringInputTargetImpl.hpp"
#include "Ui/Misc/SerializationFilter.hpp"


namespace App::Ui { class WindowElement;}
namespace App::Ui::States { class UiState; }


namespace App::Ui::User
{
	class ShaderEditorFrontend final : public Core::UiFrontend
	{
		private: States::UiState *parent;
		
		private: assetSystem::AssetPtrTyped<Assets::ShaderAsset> shaderToEdit;

		private: std::string shaderName;

		private: StringInputTargetImpl<Filter::SerializationFilter> codeToEdit;

		private: WindowElement *window;
		
		private: bool hasUnsavedChanges;
		
		private: bool shouldSave;

		private: bool shouldAbort;

		

		public: ShaderEditorFrontend(States::UiState &parent, std::string &&shaderName, const assetSystem::AssetPtr &shaderToEdit);
		
		public: void Update(Core::UiBuilder &builder) override;

		public: Core::StringInputTarget *GetInputTargetString(size_t index) override;

		public: bool *GetInputTargetBool(size_t index) override;
		
		
	};
	
	
}
