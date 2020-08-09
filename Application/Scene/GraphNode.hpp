#pragma once
#include "Types/Matrix.hpp"
#include "GraphNodeContent.hpp"
#include "Shared/PtrTypes.hpp"
#include <vector>


namespace App::Scene
{
	class GraphVisitor;
	
	class GraphNode
	{
		private: Math::Matrix nodeTransform;

		private: std::vector<GraphNode> children;

		private: UniquePtr<GraphNodeContent> content;

		private: std::string displayName;
		

		
		public: GraphNode() = default;

		public: GraphNode(std::string &&displayName, const Math::Matrix &nodeTransform, UniquePtr<GraphNodeContent> &&content);
		
		
		public: const Math::Matrix &GetNodeTransform() const { return nodeTransform; }
		
		public: uint32_t GetChildCount() const { return children.size(); }

		public: GraphNode &GetChildRefAt(uint32_t index) { return children.at(index); }

		public: const char *GetDisplayName() const { return displayName.c_str(); }

		public: GraphNodeContent *GetContent() { return content.get(); }
						
		public: void Accept(GraphVisitor &visitor);

		public: void ContentAccept(GraphVisitor &visitor);
		
		public: void ChildrenAccept(GraphVisitor &visitor);

		public: void AddChild(const char *displayName, const Math::Matrix &childTransform, UniquePtr<GraphNodeContent> &&content);

		public: void ClearChildren();
		
		public: UniquePtr<Ui::Core::UiFrontend> GetTransformEditor(Ui::Core::UiLayoutElement &parentLayout);
							   							   		
	};

	
}
