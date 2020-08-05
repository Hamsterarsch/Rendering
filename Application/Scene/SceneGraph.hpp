#pragma once
#include "Types/Vector.hpp"
#include "GraphNode.hpp"


namespace App::Scene
{
	class GraphNode;

	class SceneGraph
	{
		private: GraphNode rootNode;
		
		private: Math::Vector3 lastOriginPos;
				 		
		private: static constexpr uint32_t manhattanDistanceFromOriginPos{ 10'000 };
		

		
		public: SceneGraph();
		
		public: void SetCameraPos(float x, float y, float z);

			private: void CorrectScenePosition();

		public: void Accept(GraphVisitor &visitor);
													   		
	};
	
	
}
