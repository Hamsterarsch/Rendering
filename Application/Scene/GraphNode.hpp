#pragma once
#include "Types/Matrix.hpp"
#include "GraphNodeContent.hpp"
#include "Shared/PtrTypes.hpp"
#include <forward_list>


namespace App::Scene
{
	class GraphVisitor;
	
	class GraphNode
	{
		private: Math::Matrix nodeTransform;

		private: std::forward_list<GraphNode> children;

		private: UniquePtr<GraphNodeContent> content;


		public: GraphNode() = default;

		public: GraphNode(const Math::Matrix &nodeTransform, UniquePtr<GraphNodeContent> &&content);
		
		
		public: const Math::Matrix &GetNodeTransform() const { return nodeTransform; }
		
		public: void Accept(GraphVisitor &visitor);

		public: void ContentAccept(GraphVisitor &visitor);
		
		public: void ChildrenAccept(GraphVisitor &visitor);

		public: void AddChild(const Math::Matrix &childTransform, UniquePtr<GraphNodeContent> &&content);

		public: void ClearChildren();
		
							   		
	};

	
}
