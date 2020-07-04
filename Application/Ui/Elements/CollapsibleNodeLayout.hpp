#pragma once
#include "Ui/Core/UiLayoutElement.hpp"


namespace App::Ui
{
	namespace Core { class UiFrontend; }
	class CollapsibleNodeLayoutFrontend;
	
	class CollapsibleNodeLayout final : public Core::UiLayoutElement
	{
		private: inline static constexpr float headerSpacing{ 5 };
		
		private: UniquePtr<UiElement> headerElement;

		private: UniquePtr<UiElement> child;

		private: float indent;

		

		public: CollapsibleNodeLayout(Core::UiFrontend &frontend, size_t isOpenIndex, const char *header, float indent = 10);

		public: CollapsibleNodeLayout(CollapsibleNodeLayoutFrontend &frontend, const char *header, float indent = 10);
		
		
		public: void AddChild(UniquePtr<UiElement> &&child) override;
				   
		public: void ClearChildren() override;

		protected: void RenderAndQueryInternal(Core::UiBuilder &builder) override;

		public: void SetChildIsHidden(bool value) { child->SetIsHidden(value); }
				   		
	};
		
		
}
