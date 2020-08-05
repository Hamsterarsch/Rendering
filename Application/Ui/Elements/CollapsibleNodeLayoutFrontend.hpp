#pragma once
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/UiFrontend.hpp"
#include "CollapsibleNodeLayout.hpp"


namespace App::Ui
{
	class CollapsibleNodeLayoutFrontend final : public Core::UiFrontend
	{
		private: bool isHeaderNodeToggledOn;

		private: CollapsibleNodeLayout *targetTree;

		private: UniquePtr<UiFrontend> childFrontend;
		

		
		public: CollapsibleNodeLayoutFrontend() : CollapsibleNodeLayoutFrontend{{}} {}
					
		public: CollapsibleNodeLayoutFrontend(UniquePtr<UiFrontend> &&childFrontend) : isHeaderNodeToggledOn{ true }, targetTree{ nullptr }, childFrontend{ std::move(childFrontend)} {}

		
		public: void SetTargetNode(CollapsibleNodeLayout &tree) { targetTree = &tree; }
		
		public: bool *GetInputTargetBool(const size_t index) override { return &isHeaderNodeToggledOn; }

		public: void Update(Core::UiBuilder &builder) override;
		
		public: UiFrontend *GetChildFrontend() { return childFrontend.get(); }
							   				 		
	};
	

}
