#pragma once
#include "Shared/PtrTypes.hpp"
#include "Ui/Core/UiElement.hpp"
#include <vector>


namespace App::Ui::Core
{	
	class UiLayoutElement : public UiElement
	{		
		private: std::vector<UniquePtr<UiElement>> children;


		
		protected: size_t GetChildCount() const { return children.size(); }
		
		public: void AddChild(UniquePtr<UiElement> &&child);

		protected: virtual void OnChildAdded(UiElement &child) {}
							   				 
		public: void RenderAndQueryInternal(UiBuilder &builder) final override;

		protected: virtual void OnPreRenderAndQueryChildren(UiBuilder &builder) {}
		
		protected: virtual void OnPreRenderAndQueryChild(UiBuilder &builder, size_t childIndex, UiElement &child) {}

		protected: virtual void OnPostRenderAndQueryChild(UiBuilder &builder, size_t childIndex, UiElement &child) {}

		protected: virtual void OnPostRenderAndQueryChildren(UiBuilder &builder) {}
		
	};

	
}
