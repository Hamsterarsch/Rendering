#pragma once

#include "Ui/Core/UiBuilder.hpp"
#include <string>
#include "Ui/Core/UiLayoutElementBasic.hpp"
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui
{
	class TreeLayoutFrontend;
	
	class TreeLayout final : public Core::UiLayoutElement
	{
		private: inline static constexpr float headerSpacing{ 5 };
		
		private: UniquePtr<UiElement> headerElement;

		private: UniquePtr<UiElement> child;

		private: float indent;

		

		public: TreeLayout(Core::UiFrontend &frontend, size_t isOpenIndex, const char *header, float indent = 10);

		public: TreeLayout(TreeLayoutFrontend &frontend, const char *header, float indent = 10);
		
		
		public: void AddChild(UniquePtr<UiElement> &&child) override;
				   
		public: void ClearChildren() override;

		protected: void RenderAndQueryInternal(Core::UiBuilder &builder) override;

		public: void SetChildIsHidden(bool value) { child->SetIsHidden(value); }
				   		
	};



	class TreeLayoutFrontend final : public Core::UiFrontend
	{
		private: bool isHeaderNodeToggledOn;

		private: TreeLayout *targetTree;
		

		public: TreeLayoutFrontend() : isHeaderNodeToggledOn{ true }, targetTree{ nullptr } {}

		public: void SetTargetTree(TreeLayout &tree) { targetTree = &tree; }
		
		public: void Update(Core::UiBuilder &builder) override
		{
			if(targetTree)
			{
				targetTree->SetChildIsHidden(not isHeaderNodeToggledOn);				
			}
						
		}


		bool *GetInputTargetBool(size_t index) override
		{
			return &isHeaderNodeToggledOn;
			
		}
		
	};
		
		
}
