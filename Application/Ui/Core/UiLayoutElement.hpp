#pragma once
#include "Shared/PtrTypes.hpp"
#include "Ui/Core/UiElement.hpp"
#include <vector>


namespace App::Ui
{

	
	class UiLayoutElement : public UiElement
	{		
		private: std::vector<UniquePtr<UiElement>> children;


		
		public: void AddChild(UniquePtr<UiElement> &&child);

		protected: virtual void OnChildAdded(UiElement &child) {}
							   				 
		public: void RenderAndQueryInput(UiBuilder &builder) final override;

		protected: virtual void OnPreRenderAndQueryChildren(UiBuilder &builder) {}
		
		protected: virtual void OnPreRenderAndQueryChild(UiBuilder &builder) {}

		protected: virtual void OnPostRenderAndQueryChild(UiBuilder &builder) {}

		protected: virtual void OnPostRenderAndQueryChildren(UiBuilder &builder) {}
		
	};

	
}
