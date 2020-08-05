#include "Scene/GraphNode.hpp"
#include "Scene/GraphVisitor.hpp"
#include "Ui/User/TransformEditorFrontend.hpp"


namespace App::Scene
{
	GraphNode::GraphNode(std::string &&displayName, const Math::Matrix &nodeTransform, UniquePtr<GraphNodeContent> &&content)
		:
		nodeTransform{ nodeTransform },
		content{ std::move(content) },
		displayName{ std::move(displayName) }
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


	
	void GraphNode::AddChild(const char *displayName, const Math::Matrix &childTransform, UniquePtr<GraphNodeContent> &&content)
	{
		children.emplace_back(displayName, childTransform, std::move(content));
		
	}


	
	void GraphNode::ClearChildren()
	{
		children.clear();
		
	}


	
	UniquePtr<Ui::Core::UiFrontend> GraphNode::GetTransformEditor(Ui::Core::UiLayoutElement &parentLayout)
	{
		if(content)
		{
			if(auto editor{ content->MakeTransformEditor(nodeTransform, parentLayout) })
			{
				return editor;
			}			
		}

		return MakeUnique<Ui::User::TransformEditorFrontend>(nodeTransform, &parentLayout);
		
	}

	
}
