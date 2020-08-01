#pragma once
#include "Math/Interface/Types/Matrix.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Shared/PtrTypes.hpp"
#include "ShaderRelevantTypes/GlobalBufferData.hpp"

//use frwd decl
#include "Commands/CommandFactory.hpp"
#include <vector>
#include "GraphVisitorHarvestMeshes.hpp"
#include "VolumeTileGrid.hpp"


namespace assetSystem { class AssetSystem; }
namespace Renderer
{
	class RendererFacade;
	namespace Commands { class CompositeCommand; }
}


namespace App::Rendering
{
	class RendererMediator;
	class GraphVisitorHarvestMeshes;
	
	struct PipelineData
	{
		Renderer::HandleWrapper signature;
		Renderer::HandleWrapper pso;
	};
		
	class SceneRenderer
	{
		private: RendererMediator *mediator;

		private: GlobalBufferData globalData;

		private: Renderer::HandleWrapper globalDataBuffer;
		
		private: Renderer::HandleWrapper globalDataBufferDescriptor;
		
		private: PipelineData pipelineDepthOnly;
		
		private: PipelineData pipelineCreateVolumeTileGrid;

		private: PipelineData pipelineMarkActiveVolumeTiles;

		private: PipelineData pipelineBuildActiveVolumeTileList;

		private: PipelineData pipelineAssignLightsToTiles;

		private: UniquePtr<Renderer::Commands::CommandFactory> commandFactory;

		private: VolumeTileGrid currentVolumeTileGrid;
		
		private: Renderer::HandleWrapper gridDataBuffer;

		private: unsigned gridDataBufferSizeInBytes;

		private: Renderer::HandleWrapper gridBoundingBoxBuffer;

		private: bool volumeTileGridIsInvalidated;

		private: std::vector<Math::Matrix> currentTransformData;

		private: std::vector<uint32_t> currentTransformIndexData;
		
		private: UniquePtr<Renderer::Commands::CompositeCommand> currentDrawCommands;
		
		
		private: static constexpr float nearDistance{ .5 };

		private: static constexpr uint32_t farDistance{ 5'000 };

		private: static constexpr float verticalFovDegrees{ 90 };

		private: static constexpr float tileDepthScale{ 1.30 };
		


		public: SceneRenderer();
		
		public: SceneRenderer
		(
			RendererMediator &mediator,
			Renderer::RendererFacade &renderer,
			assetSystem::AssetSystem &internalShaderProvider, 
			const Math::VectorUint2 &surfaceDimensions
		);
				
			private: static PipelineData MakeDepthOnlyPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);
		
			private: static PipelineData MakeVolumeTileGridCreationPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);

			private: static PipelineData MakeMarkActiveVolumeTilesPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);

			private: static PipelineData MakeBuildActiveVolumeTileListPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);

			private: static PipelineData MakeAssignLightsToTilesPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);

			private: void UpdateProjection(const Math::VectorUint2 &screenDimensions);
		
			private: void SubmitCommandsToFitVolumeTileGridToProjection(Renderer::RendererFacade &renderer);


		public: SceneRenderer(const SceneRenderer &) = delete;

		public: SceneRenderer &operator=(const SceneRenderer &) = delete;

		public: SceneRenderer(SceneRenderer &&) noexcept;

		public: SceneRenderer &operator=(SceneRenderer &&) noexcept;
		
		public: ~SceneRenderer();


		public: void SubmitFrame(const UniquePtr<GraphVisitorHarvestMeshes> &graphData);

			private: void AddMeshBindingToCurrentCommands(assetSystem::AssetPtrTyped<Assets::StaticMeshAsset> &mesh);

		private: void AddMeshShardDrawToCurrentCommands(const Assets::MeshSubgroup &shardInfo, unsigned numInstances, std::vector<MeshShardInstanceData> &instanceData, uint32_t offsetToMeshTransforms);

			private: void AppendMeshTransformsToTransformData(const std::vector<Math::Matrix> &meshTransforms);

		private: void UploadGlobalBuffer(float newTime, Renderer::RendererFacade &renderer);
		
		public: void SetCamera(const Math::Vector3 &position, const Math::Vector3 &rotation);
		
		public: void OnMainWindowSurfaceSizeChanged(const Math::VectorUint2 &newSize);
				
	};

	
}
