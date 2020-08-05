#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/PtrTypes.hpp"


namespace assetSystem{ class AssetSystem; }
namespace Renderer{ class RendererFacade; }
namespace App::Ui
{
	class OutlinerInfo;
	class UiStateMachine;
}
namespace App::Scene{ class SceneGraph; }
namespace App::Assets{ class AssetTypesRegistry; }


namespace App::Core
{
	class Version;
	
	class Application
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(Application)
		
		
		public: virtual assetSystem::AssetSystem &GetProgramAssets() = 0;

		public: virtual assetSystem::AssetSystem &GetProjectAssets() = 0;

		public: virtual Renderer::RendererFacade &GetRenderer() = 0;

		public: virtual Ui::UiStateMachine &GetUiStateMachine()  = 0;

		public: virtual Version GetProgramVersion() const  = 0;
		
		public: virtual const Assets::AssetTypesRegistry &GetAssetTypes() const  = 0;
		
		public: virtual bool ProjectAssetsAreInvalid() const  = 0;

		public: virtual void SetProjectAssets(UniquePtr<assetSystem::AssetSystem> &&assets) = 0;

		public: virtual Scene::SceneGraph &GetCurrentScene() = 0;
		
	};

	
}
