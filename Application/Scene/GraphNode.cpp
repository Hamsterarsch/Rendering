#include "Scene/GraphNode.hpp"
#include "Scene/GraphVisitor.hpp"


namespace App::Scene
{
	GraphNode::GraphNode(const Math::Matrix &nodeTransform, UniquePtr<GraphNodeContent> &&content)
		:
		nodeTransform{ nodeTransform },
		content{ std::move(content) }
	{
	}


	
	void GraphNode::Accept(GraphVisitor &visitor)
	{
		visitor.Visit(*this);
		
	}


	
	void GraphNode::ContentAccept(GraphVisitor &visitor)
	{
		if(content)
		{
			content->Accept(visitor);
		}
		
	}


	
	void GraphNode::ChildrenAccept(GraphVisitor &visitor)
	{
		for(auto &&child : children)
		{
			child.Accept(visitor);			
		}
		
	}


	
	void GraphNode::AddChild(const Math::Matrix &childTransform, UniquePtr<GraphNodeContent> &&content)
	{
		children.emplace_front(childTransform, std::move(content));
		
	}


	
	void GraphNode::ClearChildren()
	{
		children.clear();
		
	}

	
}
