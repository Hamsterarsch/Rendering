#pragma once
#include "Asset.hpp"
#include "Resources/HandleWrapper.hpp"

#include "Core/Application.hpp"
#include "AssetSystem.hpp"

namespace App::Assets
{
	class ShaderAsset;

	class PsoAsset final : public assetSystem::Asset
	{
		private: Renderer::HandleWrapper psoHandle;
		

		
		public: assetSystem::io::Archive& Serialize(assetSystem::io::Archive& archive) override { return archive; }
	};

	Renderer::HandleWrapper MakePsoVolumeTileGridCreation(Core::Application &app)
	{
		//load all necessary shader assets and compile their code here
		app.GetProgramAssets().GetAsset("Shaders/")

		
		//make a signature with the required slots

		//make the pso
		
	}
	
}
