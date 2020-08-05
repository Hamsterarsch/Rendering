#include "Ui/Misc/GraphVisitorOutlinerInfo.hpp"
#include "Scene/GraphNode.hpp"


namespace App::Ui
{
	void GraphVisitorOutlinerInfo::Visit(Scene::GraphNode &node)
	{
		root.children.clear();
		AddNodeInfo(root, node);		
		
	}


	
	void GraphVisitorOutlinerInfo::AddNodeInfo(OutlinerInfo &toInfo, Scene::GraphNode &forNode)
	{
		toInfo.node = &forNode;
		
		for(uint32_t childIndex{ 0 }; childIndex < forNode.GetChildCount(); ++childIndex)
		{
			toInfo.children.emplace_back(&forNode.GetChildRefAt(childIndex));		
			AddNodeInfo(toInfo.children.back(), *toInfo.children.back().node)	;
		}//todo: check if breadth first iteration is faster
			
	}

	
}
