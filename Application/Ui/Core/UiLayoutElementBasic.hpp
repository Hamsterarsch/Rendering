#pragma once
#include "Ui/Core/UiLayoutElement.hpp"
#include <vector>


namespace App::Ui::Core
{	
	class UiLayoutElementBasic : public UiLayoutElement
	{		
		private: std::vector<UniquePtr<UiElement>> children;



		public: void ClearChildren() final override { children.clear(); }
		
		protected: size_t GetChildCount() const final override { return children.size(); }
				
		public: void AddChild(UniquePtr<UiElement> &&child) override;

		protected: virtual void OnChildAdded(UiElement &child) override{}
							   				 
		public: void RenderAndQueryInternal(UiBuilder &builder) final override;

		protected: virtual void OnPreRenderAndQueryChildren(UiBuilder &builder) override{}
		
		protected: virtual void OnPreRenderAndQueryChild(UiBuilder &builder, size_t childIndex, UiElement &child) override{}

		protected: virtual void OnPostRenderAndQueryChild(UiBuilder &builder, size_t childIndex, UiElement &child) override{}

		protected: virtual void OnPostRenderAndQueryChildren(UiBuilder &builder) override{}
		
	};

	
}
