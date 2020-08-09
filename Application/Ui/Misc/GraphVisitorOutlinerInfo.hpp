#pragma once
#include "Scene/GraphVisitor.hpp"
#include "OutlinerInfo.hpp"


namespace App::Ui
{	
	class GraphVisitorOutlinerInfo final : public Scene::GraphVisitor
	{
		public: OutlinerInfo root;

		

		public: GraphVisitorOutlinerInfo() : root{nullptr} {}
		
		public: void Visit(Scene::GraphNode &node) override;

			private: void AddNodeInfo(OutlinerInfo &toInfo, Scene::GraphNode &forNode);
		
		public: void Visit(Scene::ContentMesh &mesh) override {}
		
		public: void Visit(Scene::ContentLight &light) override {}
				
	};
	
	
}
