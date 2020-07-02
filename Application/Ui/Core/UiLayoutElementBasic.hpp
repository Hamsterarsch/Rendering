#pragma once
#include "Ui/Core/UiLayoutElement.hpp"
#include <vector>


namespace App::Ui::Core
{	
	class UiLayoutElementBasic : public UiLayoutElement
	{		
		private: std::vector<UniquePtr<UiElement>> children;



		public: void ClearChildren() final override { children.clear(); }
						
		public: void AddChild(UniquePtr<UiElement> &&child) override;

		public: void RenderAndQueryInternal(UiBuilder &builder) final override;
			
		protected: virtual void OnChildAdded(UiElement &child) {}

		protected: virtual void OnPreRenderAndQueryChildren(UiBuilder &builder) {}
		
		protected: virtual void OnPreRenderAndQueryChild(UiBuilder &builder, size_t childIndex, UiElement &child) {}

		protected: virtual void OnPostRenderAndQueryChild(UiBuilder &builder, size_t childIndex, UiElement &child) {}

		protected: virtual void OnPostRenderAndQueryChildren(UiBuilder &builder) {}
		
		protected: size_t GetChildCount() const { return children.size(); }
		
	};

	
}
