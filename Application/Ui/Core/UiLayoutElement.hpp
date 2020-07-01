#pragma once
#include "Shared/PtrTypes.hpp"
#include "Ui/Core/UiElement.hpp"


namespace App::Ui::Core
{	
	class UiLayoutElement : public UiElement
	{				
		public: virtual void ClearChildren() = 0;
		
		protected: virtual size_t GetChildCount() const = 0;
				
		public: virtual void AddChild(UniquePtr<UiElement> &&child) = 0;

		protected: virtual void OnChildAdded(UiElement &child) = 0;
							   				 
		public: virtual void RenderAndQueryInternal(UiBuilder &builder) = 0;

		protected: virtual void OnPreRenderAndQueryChildren(UiBuilder &builder) = 0;
		
		protected: virtual void OnPreRenderAndQueryChild(UiBuilder &builder, size_t childIndex, UiElement &child) = 0;

		protected: virtual void OnPostRenderAndQueryChild(UiBuilder &builder, size_t childIndex, UiElement &child) = 0;

		protected: virtual void OnPostRenderAndQueryChildren(UiBuilder &builder) = 0;
		
	};

	
}
