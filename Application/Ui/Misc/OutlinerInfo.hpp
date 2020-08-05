#pragma once
#include <vector>


namespace App::Scene
{
	class GraphNode;
}


namespace App::Ui
{
	struct OutlinerInfo
	{
		Scene::GraphNode *node;
		
		std::vector<OutlinerInfo> children;

		public: OutlinerInfo(Scene::GraphNode *node) : node{ node } {}
							   				 						
	};

	
}
